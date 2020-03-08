#include "LayeredNetwork/Layers/ConvolutionalLayer.hpp"

Matrix* ConvolutionalLayer::convolute(Matrix* input, Matrix* filter, Matrix* output, int s, bool flipped) {
    return convolute(input, filter, 0, output, s, flipped, NULL);
}

Matrix* ConvolutionalLayer::convolute(Matrix* input, Matrix* filter, Matrix* output, int s, bool flipped, Matrix* adder) {
    return convolute(input, filter, 0, output, s, flipped, adder);
}

Matrix* ConvolutionalLayer::convolute(Matrix* input, Matrix* filter, double bias, Matrix* output, int s, bool flipped, Matrix* adder) {
    double outputRows = ((input->getNRows() - filter->getNRows()) / ((double) s)) + 1.0;
    double outputCols = ((input->getNCols() - filter->getNRows()) / ((double) s)) + 1.0;

    if((int) outputRows != outputRows || (int) outputCols != outputCols)
        throw std::invalid_argument("Invalid convolution parameters");

    if((int) outputRows != output->getNRows() || (int) outputCols != output->getNCols())
        throw std::invalid_argument("convolution parameters don't match paramater constraints");

    for(int row = 0; row <= input->getNRows() - filter->getNRows(); row += s) {
    for(int col = 0; col <= input->getNCols() - filter->getNRows(); col += s) {
        double sum = 0.0;
        if(flipped)
            sum = input->segmentedFlippedElementWiseProductSum(filter, row, col);
        else
            sum = input->segmentedElementWiseProductSum(filter, row, col);

        sum += bias;

        if(adder == NULL)
            output->set(row / s, col / s, sum);
        else
            output->set(row / s, col / s, sum + adder->at(row / s, col / s));
    }}

    return output;
}

ConvolutionalLayer::ConvolutionalLayer(NetworkInformation& networkInformation, UpdaterID updaterID, int index, int kernalCount, int kernalSize, int stride) 
    : Layer(networkInformation, Conv, index), updaterID(updaterID), kernalSize(kernalSize), stride(stride) {

    outputMatrixCount = kernalCount;
}

void ConvolutionalLayer::initialize() {
    parameterGradientInfoLength = parameterLength = (kernalSize * kernalSize * outputMatrixCount * inputMatrixCount) + outputMatrixCount;

    parameters = (double*) calloc(parameterLength, sizeof(double));
    parameterGradientInfo = (double*) calloc(parameterGradientInfoLength, sizeof(double));

    weights = new Matrix*[outputMatrixCount];
    weightGradient = new Matrix*[outputMatrixCount];
    weightUpdater = new Updater**[outputMatrixCount];
    for(int i = 0; i < outputMatrixCount; i++) {
        weights[i] = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
        weightGradient[i] = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
        weightUpdater[i] = new Updater*[inputMatrixCount]; //malloc(sizeof(MomentumUpdater) * inputMatrixCount);

        for(int j = 0; j < inputMatrixCount; j++) {
            new (&(weights[i][j])) Matrix(&(parameters[((i * inputMatrixCount) + j) * kernalSize * kernalSize]), kernalSize, kernalSize, -1.0, 1.0);
            new (&(weightGradient[i][j])) Matrix(&(parameterGradientInfo[((i * inputMatrixCount) + j) * kernalSize * kernalSize]), kernalSize, kernalSize);
            weightUpdater[i][j] = createUpdaterFromID(updaterID, networkInformation, kernalSize, kernalSize).release();
            // new (&(weightUpdater[i][j])) MomentumUpdater(networkInformation, kernalSize, kernalSize);
        }
    }

    bias = new Matrix(&(parameters[parameterLength - outputMatrixCount]), 1, outputMatrixCount, -1.0, 1.0);
    biasGradient = new Matrix(&(parameterGradientInfo[parameterLength - outputMatrixCount]), 1, outputMatrixCount);
    biasUpdater = createUpdaterFromID(updaterID, networkInformation, 1, outputMatrixCount).release();//new MomentumUpdater(networkInformation, 1, outputMatrixCount);

    double outputRows = ((inputNRows - kernalSize) / ((double) stride)) + 1.0;
    double outputCols = ((inputNCols - kernalSize) / ((double) stride)) + 1.0;

    if((int) outputRows != outputRows || (int) outputCols != outputCols)
        throw std::invalid_argument("Invalid constructor convolution paramaters");

    outputNRows = (int) outputRows;
    outputNCols = (int) outputCols;

    outputInfoLength = outputNRows * outputNCols * outputMatrixCount;
    outputInfo = (double*) calloc(outputInfoLength, sizeof(double));

    output = (Matrix*) malloc(sizeof(Matrix) * outputMatrixCount);
    for(int i = 0; i < outputMatrixCount; i++)
        new (&output[i]) Matrix(&outputInfo[outputNRows * outputNCols * i], outputNRows, outputNCols);

    errorDialatedPadded = new Matrix(((outputNRows - 1) * (stride - 1)) + outputNRows + (2 * (kernalSize - 1)), 
                                        ((outputNCols - 1) * (stride - 1)) + outputNCols + (2 * (kernalSize - 1)));

    if(index != 0) {
        input = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);

        for(int i = 0; i < inputMatrixCount; i++)
            new (&input[i]) Matrix(&(networkInformation.getLayers()[index - 1]->getOutputInfo()[inputNCols * inputNRows * i]), inputNRows, inputNCols);

        layerGradient = (Matrix*) malloc(sizeof(Matrix) * inputMatrixCount);
        layerGradientInfoLength = networkInformation.getLayers()[index - 1]->getOutputInfoLength();
        layerGradientInfo = (double*) calloc(layerGradientInfoLength, sizeof(double));

        for(int i = 0; i < inputMatrixCount; i++)
            new (&layerGradient[i]) Matrix(&layerGradientInfo[inputNRows * inputNCols * i], inputNRows, inputNCols);
    }
}

void ConvolutionalLayer::postInitialize() {
    if(index != networkInformation.getAmountOfLayers() - 1) {
        error = (Matrix*) malloc(sizeof(Matrix) * outputMatrixCount);
        for(int i = 0; i < outputMatrixCount; i++)
            new (&error[i]) Matrix(&(networkInformation.getLayers()[index + 1]->getLayerGradientInfo()[outputNRows * outputNCols * i]), outputNRows, outputNCols);
    }
}

void ConvolutionalLayer::writeConstructInfo(FILE* file) {
    int updater = (int) updaterID;
    fwrite(&updater, sizeof(int), 1, file);

    fwrite(&outputMatrixCount, sizeof(int), 1, file);
    fwrite(&kernalSize, sizeof(int), 1, file);
    fwrite(&stride, sizeof(int), 1, file);
}

void ConvolutionalLayer::writeState(FILE* file) {
    for(int i = 0; i < outputMatrixCount; i++) {
    for(int j = 0; j < inputMatrixCount; j++) {
        weightUpdater[i][j]->writeState(file);
    }}

    biasUpdater->writeState(file);
}

void ConvolutionalLayer::loadState(FILE* file) {
    for(int i = 0; i < outputMatrixCount; i++) {
    for(int j = 0; j < inputMatrixCount; j++) {
        weightUpdater[i][j]->loadState(file);
    }}

    biasUpdater->loadState(file);
}

Matrix* ConvolutionalLayer::feedForward() {
    for(int i = 0; i < outputMatrixCount; i++) {
        for(int inputMatrixIndex = 0; inputMatrixIndex < inputMatrixCount; inputMatrixIndex++) {
            if(inputMatrixIndex == 0)
                convolute(&input[inputMatrixIndex], &weights[i][inputMatrixIndex], bias->at(0, i), &output[i], stride, false, NULL);
            else
                convolute(&input[inputMatrixIndex], &weights[i][inputMatrixIndex], bias->at(0, i), &output[i], stride, false, &output[i]);
        }
    }

    return output;
}

Matrix* ConvolutionalLayer::calculateGradient() {
    if(index != 0) {
        //Layer Gradient Calculation
        for(int i = 0; i < inputMatrixCount; i++) {
            for(int j = 0; j < outputMatrixCount; j++) {
                (&error[j])->dialatePad(stride, kernalSize - 1, errorDialatedPadded);

                if(i == 0 && j == 0)
                    convolute(errorDialatedPadded, &weights[j][i], (&layerGradient[i]), 1, true);
                else
                    convolute(errorDialatedPadded, &weights[j][i], (&layerGradient[i]), 1, true, (&layerGradient[i]));
            }
        }
    }

    //Weight Gradient Calculation
    for(int i = 0; i < outputMatrixCount; i++) {
        for(int j = 0; j < inputMatrixCount; j++) {
            if(networkInformation.getBatchIndex() == 0)
                convolute(&input[j], &error[i], &(weightGradient[i][j]), (int) (input->getNRows() - error->getNRows()) / (kernalSize - 1), false);
            else
                convolute(&input[j], &error[i], &(weightGradient[i][j]), (int) (input->getNRows() - error->getNRows()) / (kernalSize - 1), false, &(weightGradient[i][j]));
        }

        if(networkInformation.getBatchIndex() == 0)
            biasGradient->getData()[i] = (&error[i])->sumElements();
        else
            biasGradient->getData()[i] += (&error[i])->sumElements();
    }

    return layerGradient;
}

void ConvolutionalLayer::update() {
    for(int i = 0; i < outputMatrixCount; i++)
    for(int j = 0; j < inputMatrixCount; j++)
        weightUpdater[i][j]->update(&weights[i][j], &weightGradient[i][j]);

    biasUpdater->update(bias, biasGradient);
}