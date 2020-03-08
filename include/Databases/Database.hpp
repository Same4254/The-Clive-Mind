#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <netinet/in.h>

#ifndef DATABASE
#define DATABASE

class Database {
public:
    int epochSize;
    int testSize;

    virtual double* getTrainingData(int index) = 0;
    virtual double* getTrainingLabel(int index) = 0;

    virtual double* getTestData(int index) = 0;
    virtual double* getTestLabel(int index) = 0;

    int getEpochSize();
    int getTestSize();
};
#endif