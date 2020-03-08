#include "Databases/Database.hpp"

#ifndef MNIST_DATABASE
#define MNIST_DATABASE

class MnistDatabase : public Database {
private:
    static const int TRAIN_IMAGE_COUNT = 60000, TEST_IMAGE_COUNT = 10000;

    double oneHotTestLabels[TEST_IMAGE_COUNT][10];
    unsigned char testImagesBuffer[TEST_IMAGE_COUNT][784];

    double oneHotTrainLabels[TRAIN_IMAGE_COUNT][10];
    unsigned char trainImagesBuffer[TRAIN_IMAGE_COUNT][784];

    unsigned char trainLabelsBuffer[TRAIN_IMAGE_COUNT];
    unsigned char testLabelsBuffer[TEST_IMAGE_COUNT];

    double testImages[TEST_IMAGE_COUNT][784];
    double trainImages[TRAIN_IMAGE_COUNT][784];

    bool loadLabels(FILE* fd, unsigned int count, unsigned char* labels);
    bool loadImages(FILE* fd, unsigned int count, unsigned char (*images)[784]);
    bool loadDataset();

public:
    MnistDatabase();

    double* getTrainingData(int index);
    double* getTrainingLabel(int index);

    double* getTestData(int index);
    double* getTestLabel(int index);
};
#endif