#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <netinet/in.h>

#define TEST_LABEL_COUNT 10000
#define TEST_IMAGE_COUNT 10000

#define TRAIN_LABEL_COUNT 60000
#define TRAIN_IMAGE_COUNT 60000

#define IMAGE_SIZE 784

unsigned char testLabels[TEST_LABEL_COUNT];
unsigned char testImagesBuffer[TEST_IMAGE_COUNT][IMAGE_SIZE];

unsigned char trainLabels[TRAIN_LABEL_COUNT];
unsigned char trainImagesBuffer[TRAIN_IMAGE_COUNT][IMAGE_SIZE];

double testImages[TEST_IMAGE_COUNT][IMAGE_SIZE];
double trainImages[TRAIN_IMAGE_COUNT][IMAGE_SIZE];

bool loadLabels(FILE* fd, int count, unsigned char* labels) {
    int32_t buff[2];//magic number and label count

    if(fread(buff, sizeof(int32_t), 2, fd) != 2)
        return false;

    if(fread(labels, 1, count, fd) != count)
        return false;

    return true;
}

bool loadImages(FILE* fd, int count, unsigned char (*images)[IMAGE_SIZE]) {
    int32_t buff[4];//magic number, image count, image length, image width

    if(fread(buff, sizeof(int32_t), 4, fd) != 4)
        return false;

    for(int i = 0; i < count; i++)
        if(fread(images[i], 1, IMAGE_SIZE, fd) != IMAGE_SIZE)
            return false;

    return true;
}

bool loadDataset() {
    FILE* testLabelFD = fopen("Mnist/Data/t10k-labels.idx1-ubyte", "r");
    FILE* testImageFD = fopen("Mnist/Data/t10k-images.idx3-ubyte", "r");
    FILE* trainLabelFD = fopen("Mnist/Data/train-labels.idx1-ubyte", "r");
    FILE* trainImageFD = fopen("Mnist/Data/train-images.idx3-ubyte", "r");

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