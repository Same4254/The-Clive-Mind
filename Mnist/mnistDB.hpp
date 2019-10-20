#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <netinet/in.h>

using namespace std;

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

void loadLabels(FILE* fd, int count, unsigned char* labels) {
    int32_t buff[2];//magic number and label count
    fread(buff, sizeof(int32_t), 2, fd);

    fread(labels, 1, count, fd);
}

void loadImages(FILE* fd, int count, unsigned char (*images)[IMAGE_SIZE]) {
    int32_t buff[4];//magic number, image count, image length, image width
    fread(buff, sizeof(int32_t), 4, fd);

    for(int i = 0; i < count; i++) {
        fread(images[i], 1, IMAGE_SIZE, fd);
    }
}

void loadDataset() {
    FILE* testLabelFD = fopen("./Data/t10k-labels.idx1-ubyte", "r");
    FILE* testImageFD = fopen("./Data/t10k-images.idx3-ubyte", "r");
    FILE* trainLabelFD = fopen("./Data/train-labels.idx1-ubyte", "r");
    FILE* trainImageFD = fopen("./Data/train-images.idx3-ubyte", "r");

    loadLabels(testLabelFD, TEST_LABEL_COUNT, testLabels);
    loadImages(testImageFD, TEST_IMAGE_COUNT, testImagesBuffer);

    loadLabels(trainLabelFD, TRAIN_LABEL_COUNT, trainLabels);
    loadImages(trainImageFD, TRAIN_IMAGE_COUNT, trainImagesBuffer);

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
}