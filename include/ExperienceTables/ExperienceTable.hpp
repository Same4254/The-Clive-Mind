#ifndef EXPERIENCE_TABLE_HPP
#define EXPERIENCE_TABLE_HPP

#include <string.h> 
#include <vector>
#include <memory>

class Experience {
private:
    double* state;
    double* resultingState;
    double reward;
    int action;

public:
    Experience(double* state, double* resultingState, int stateLength, double reward, int action);
    ~Experience();

    double* getState();
    double* getResultingState();
    double getReward();
    int getAction();
};

class ExperienceTable {
private:
    std::vector<Experience*> experiences;

public:
    ExperienceTable();
    virtual ~ExperienceTable();

    Experience* addExperience(double* state, double* resultingState, int stateLength, double reward, int action);
    Experience* getRandomExperience();
    std::vector<Experience*>& getExperiences();
};
#endif