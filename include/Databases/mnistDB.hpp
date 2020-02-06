#ifndef MNIST_DB_HPP
#define MNIST_DB_HPP

#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <netinet/in.h>

#define TEST_LABEL_COUNT 10000
#define TEST_IMAGE_COUNT 10000

#define TRAIN_LABEL_COUNT 60000
#define TRAIN_IMAGE_COUNT 60000

#define IMAGE_SIZE 784

extern unsigned char testLabels[TEST_LABEL_COUNT];
extern unsigned char testImagesBuffer[TEST_IMAGE_COUNT][IMAGE_SIZE];

extern unsigned char trainLabels[TRAIN_LABEL_COUNT];
extern unsigned char trainImagesBuffer[TRAIN_IMAGE_COUNT][IMAGE_SIZE];

extern double testImages[TEST_IMAGE_COUNT][IMAGE_SIZE];
extern double trainImages[TRAIN_IMAGE_COUNT][IMAGE_SIZE];

bool loadLabels(FILE* fd, int count, unsigned char* labels);
bool loadImages(FILE* fd, int count, unsigned char (*images)[IMAGE_SIZE]);
bool loadDataset();
#endif