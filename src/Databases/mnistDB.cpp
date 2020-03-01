#include "Databases/mnistDB.hpp"

unsigned char testLabels[TEST_LABEL_COUNT];
unsigned char testImagesBuffer[TEST_IMAGE_COUNT][IMAGE_SIZE];

unsigned char trainLabels[TRAIN_LABEL_COUNT];
unsigned char trainImagesBuffer[TRAIN_IMAGE_COUNT][IMAGE_SIZE];

double testImages[TEST_IMAGE_COUNT][IMAGE_SIZE];
double trainImages[TRAIN_IMAGE_COUNT][IMAGE_SIZE];

bool loadLabels(FILE* fd, unsigned int count, unsigned char* labels) {
    int32_t buff[2];//magic number and label count

    if(fread(buff, sizeof(int32_t), 2, fd) != 2)
        return false;

    if(fread(labels, 1, count, fd) != count)
        return false;

    return true;
}

bool loadImages(FILE* fd, unsigned int count, unsigned char (*images)[IMAGE_SIZE]) {
    int32_t buff[4];//magic number, image count, image length, image width

    if(fread(buff, sizeof(int32_t), 4, fd) != 4)
        return false;

    for(unsigned int i = 0; i < count; i++)
        if(fread(images[i], 1, IMAGE_SIZE, fd) != IMAGE_SIZE)
            return false;

    return true;
}

bool loadDataset() {
    FILE* testLabelFD = fopen("Data/t10k-labels.idx1-ubyte", "r");
    FILE* testImageFD = fopen("Data/t10k-images.idx3-ubyte", "r");
    FILE* trainLabelFD = fopen("Data/train-labels.idx1-ubyte", "r");
    FILE* trainImageFD = fopen("Data/train-images.idx3-ubyte", "r");

    if(testLabelFD == NULL || testImageFD == NULL || trainLabelFD == NULL || trainImageFD == NULL)
        return false;

    if(!loadLabels(testLabelFD, TEST_LABEL_COUNT, testLabels))
        return false;

    if(!loadImages(testImageFD, TEST_IMAGE_COUNT, testImagesBuffer))
        return false;

    if(!loadLabels(trainLabelFD, TRAIN_LABEL_COUNT, trainLabels))
        return false;

    if(!loadImages(trainImageFD, TRAIN_IMAGE_COUNT, trainImagesBuffer))
        return false;

    for(int i = 0; i < TEST_IMAGE_COUNT; i++) {
    for(int j = 0; j < 784; j++) {
        testImages[i][j] = testImagesBuffer[i][j] / 255.0;
    }}

    for(int i = 0; i < TRAIN_IMAGE_COUNT; i++) {
    for(int j = 0; j < 784; j++) {
        trainImages[i][j] = trainImagesBuffer[i][j] / 255.0;
    }}

    fclose(testLabelFD);
    fclose(testImageFD);
    fclose(trainImageFD);
    fclose(trainLabelFD);

    return true;
}