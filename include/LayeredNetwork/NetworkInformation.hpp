#ifndef NETWORK_INFORMATION_HPP
#define NETWORK_INFORMATION_HPP

#include <vector>
#include <memory>

//Circular Dependency
class Layer;

class NetworkInformation {
private:
    double learningRate, velocityCoefficient;
    int batchSize, batchIndex;
    std::vector<std::unique_ptr<Layer>>& layers;

public:
    NetworkInformation(std::vector<std::unique_ptr<Layer>>& layers);
    NetworkInformation(std::vector<std::unique_ptr<Layer>>& layers, NetworkInformation& informationToCopy);
    ~NetworkInformation();

    int getAmountOfLayers();
    std::vector<std::unique_ptr<Layer>>& getLayers();

    double getLearningRate();
    void setLearningRate(double learningRate);

    double getVelocityCoefficient();
    void setVelocityCoefficient(double velocityCoefficient);

    int getBatchSize();
    void setBatchSize(int batchSize);

    bool incrementBatchIndex();
    int getBatchIndex();
};
#endif