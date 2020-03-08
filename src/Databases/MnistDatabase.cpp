#include "Databases/MnistDatabase.hpp"

MnistDatabase::MnistDatabase() {
    epochSize = TRAIN_IMAGE_COUNT;
    testSize = TEST_IMAGE_COUNT;

    loadDataset();
}

double* MnistDatabase::getTrainingData(int index) {
    return trainImages[index];
}

double* MnistDatabase::getTrainingLabel(int index) {
    return oneHotTrainLabels[index];
}

double* MnistDatabase::getTestData(int index) {
    return testImages[index];
}

double* MnistDatabase::getTestLabel(int index) {
    return oneHotTestLabels[index];
}

bool MnistDatabase::loadLabels(FILE* fd, unsigned int count, unsigned char* labels) {
    int32_t buff[2];//magic number and label count

    if(fread(buff, sizeof(int32_t), 2, fd) != 2)
        return false;

    if(fread(labels, 1, count, fd) != count)
        return false;

    return true;
}

bool MnistDatabase::loadImages(FILE* fd, unsigned int count, unsigned char (*images)[784]) {
    int32_t buff[4];//magic number, image count, image length, image width

    if(fread(buff, sizeof(int32_t), 4, fd) != 4)
        return false;

    for(unsigned int i = 0; i < count; i++)
        if(fread(images[i], 1, 784, fd) != 784)
            return false;

    return true;
}

bool MnistDatabase::loadDataset() {
    FILE* testLabelFD = fopen("Data/t10k-labels.idx1-ubyte", "r");
    FILE* testImageFD = fopen("Data/t10k-images.idx3-ubyte", "r");
    FILE* trainLabelFD = fopen("Data/train-labels.idx1-ubyte", "r");
    FILE* trainImageFD = fopen("Data/train-images.idx3-ubyte", "r");

    if(testLabelFD == NULL || testImageFD == NULL || trainLabelFD == NULL || trainImageFD == NULL)
        return false;

    if(!loadLabels(testLabelFD, testSize, testLabelsBuffer))
        return false;

    if(!loadImages(testImageFD, testSize, testImagesBuffer))
        return false;

    if(!loadLabels(trainLabelFD, epochSize, trainLabelsBuffer))
        return false;

    if(!loadImages(trainImageFD, epochSize, trainImagesBuffer))
        return false;

    for(int i = 0; i < testSize; i++) {
    for(int j = 0; j < 784; j++) {
        testImages[i][j] = testImagesBuffer[i][j] / 255.0;
    }}

    for(int i = 0; i < epochSize; i++) {
    for(int j = 0; j < 784; j++) {
        trainImages[i][j] = trainImagesBuffer[i][j] / 255.0;
    }}

    for(int i = 0; i < TRAIN_IMAGE_COUNT; i++)
        oneHotTrainLabels[i][trainLabelsBuffer[i]] = 1;

    for(int i = 0; i < TEST_IMAGE_COUNT; i++)
        oneHotTestLabels[i][testLabelsBuffer[i]] = 1;

    fclose(testLabelFD);
    fclose(testImageFD);
    fclose(trainImageFD);
    fclose(trainLabelFD);

    return true;
}