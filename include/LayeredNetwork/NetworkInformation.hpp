#ifndef NETWORK_INFORMATION_HPP
#define NETWORK_INFORMATION_HPP

class NetworkInformation {
private:
    double learningRate, velocityCoefficient;
    int batchSize, batchIndex;
    int amountOfLayers;

public:
    NetworkInformation(int amountOfLayers);
    ~NetworkInformation();

    int getAmountOfLayers();

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