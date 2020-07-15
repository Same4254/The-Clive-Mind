#ifndef LAYER_HPP
#define LAYER_HPP

#include <stdio.h>
#include <memory>

#include <rapidjson/document.h>

#include "Matrix.hpp"
#include "LayeredNetwork/NetworkInformation.hpp"

#include "LayeredNetwork/Updaters/Updater.hpp"
#include "LayeredNetwork/Updaters/MomentumUpdater.hpp"
#include "LayeredNetwork/Updaters/AdamUpdater.hpp"
#include "LayeredNetwork/Updaters/RMSUpdater.hpp"

enum LayerID {
    Full, Conv, Act, Pool
};

enum UpdaterID {
    Momentum, Adam, RMS
};

enum ActivationID {
    Sigmoid, Relu, Softplus, LeakyRelu
};

/**
 *  This class represent the base class for any layer to extend from.
 * 
 * Some key notes to be aware of:
 *  - State information is held in contigous pointers (all of the parameters for a layer is one long pointer for example). 
 *    This was done becuase it makes sending over MPI really easy and makes copying the information to another location in memory easy
 * 
 *  - Layers can look at the pointers for such information for other layers. Why? Well this makes it trivial for one layer to create a 
 *    Matrix with different dimensions on the output values of a previous layer. This means that no information is uselessly copied,
 *    rather the next layer can simply interpret the information with different dimension sizes on the matrix (or matricies)
 * 
 * - Because of the above, layers have an *interesting* initialization process. Some matricies of a layer may use pointers of 
 *   a previous layer. As such, layers have a postInitialization function where it is safe to build refrences to other layer elements.
 *   This was done because it is possible that a layer would want to look ahead for gradient information and behind for output information of the other layers next to it.
 * 
 * - Every layer has its own ID written in an enum above this class. This is to make it simple to read and write to files about what type of layer or function is intended.
 */
class Layer {
protected:
    //Input dimensions
    int inputMatrixCount;
    int inputNRows;
    int inputNCols;

    //Output dimensions
    int outputMatrixCount;
    int outputNRows;
    int outputNCols;

    /************* State Information *************/
    int parameterLength;
    double* parameters;

    int parameterGradientInfoLength;
    double* parameterGradientInfo;

    int layerGradientInfoLength;
    double* layerGradientInfo;

    int outputInfoLength;
    double* outputInfo;

    /************* Matrix Representations *************/
    Matrix* input;
    Matrix* output;
    Matrix* layerGradient;
    Matrix* error;

    /************* Meta Information *************/
    NetworkInformation& networkInformation;
    LayerID layerID;
    int index;

    /**
     * The idea here is to build an updater pointer from just the ID (what function to update parameters with). This is used for creating the layer from a file mainly.
     * 
     * @param id -> The ID of the function to update with
     * @param parameterRows -> Number of rows in the matrix that represents the parameters to be updated
     * @param parameterCols -> Number of columns in the matrix that represents the parameters to be updated
     * 
     * @return A unique pointer to a new updater object that uses the function specified by the id
     */
    std::unique_ptr<Updater> createUpdaterFromID(UpdaterID id, int parameterRows, int parameterCols);

public:
    /**
     *  This layer must be added to the network immediately after creation, with the correct index (list of layers in the network).
     *  For an outside user, DO NOT USE THIS. Use the NetworkBuilder. It was created for a reason... 
     * 
     *  @param networkInformation -> The object for the network this layer belongs to. Stores hyperameters and other meta information
     *  @param layerID -> The ID that represents (numerically) what type of layer this is 
     */
    Layer(NetworkInformation& networkInformation, LayerID layerID, int index);
    virtual ~Layer();
    virtual void initialize();
    virtual void postInitialize() = 0;

    /**
     * Compares if two layers have the same architecture. 
     * Checks if the input and output dimensions are the same, and if they are the same type of layer (ID).
     * 
     * Essentially this asks the question, could you memcpy all the parameters with no issues?
     * 
     * @param otherLayer -> Pointer to the other layer to compare to
     * @return True if the two layers are the same (ignoring the value of parameters), false if they are not
     */
    virtual bool isEqualLayerArchitecture(std::unique_ptr<Layer>& otherLayer);

    /**
     *  Copy the state parameters of another layer.
     *  This must satisfy @see{Layer::isEqualLayerArchitecture}, for the copy to be made.
     * 
     *  The calling object is the destination. OtherLayer is the source.
     *  In other words, "this copies that"
     * 
     *  @param otherLayer -> The layer to copy the state from
     *  @return Whether the state was successfully copied
     */
    virtual bool copyState(std::unique_ptr<Layer>& otherLayer);

    virtual void writeStructureToFile(rapidjson::Value& layerJSONObject, rapidjson::Document::AllocatorType& allocator) = 0;
    virtual void writeStateToFile(FILE* file) = 0;
    virtual bool loadStateFromFile(FILE* file) = 0;

    virtual Matrix* feedForward() = 0;
    virtual Matrix* calculateGradient() = 0;
    virtual void update() = 0;

    int getParameterLength();
    double* getParameters();

    int getParameterGradientInfoLength();
    double* getParameterGradientInfo();

    Matrix* getLayerGradient();
    int getLayerGradientInfoLength();
    double* getLayerGradientInfo();

    int getInputMatrixCount();
    int getInputNRows();
    int getInputNCols();

    int getOutputMatrixCount();
    int getOutputNRows();
    int getOutputNCols();

    // void setInputMatrix(Matrix* input);
    // void setInputMatrixCount(int inputMatrixCount);
    // void setInputNRows(int inputNRows);
    // void setInputNCols(int inputNCols);

    // void setOutputMatrixCount(int outputMatrixCount);
    // void setOutputNRows(int outputNRows);
    // void setOutputNCols(int outputNCols);

    LayerID getLayerID();

    Matrix* getError();
    Matrix* getOutput();

    double* getOutputInfo();
    int getOutputInfoLength();
};
#endif