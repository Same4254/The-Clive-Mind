#ifndef EXPERIENCE_TABLE_H
#define EXPERIENCE_TABLE_H

#include <vector>
#include <memory>

class Experience {
private:
    double* state;
    double* resultingState;
    double reward;
    unsigned int action;

public:
    Experience(double* state, double* resultingState, unsigned int stateLength, double reward, unsigned int action);
    ~Experience();

    double* getState();
    double* getResultingState();
    double getReward();
    unsigned int getAction();
};

class ExperienceTable {
public:
    std::vector<std::unique_ptr<Experience>> experiences;

    ExperienceTable();
    virtual ~ExperienceTable();

    Experience* addExperience(double* state, double* resultingState, unsigned int stateLength, double reward, unsigned int action);
    Experience* getRandomExperience();
};
#endif