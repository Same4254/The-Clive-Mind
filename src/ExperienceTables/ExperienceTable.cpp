#include "ExperienceTables/ExperienceTable.hpp"

/********* Experience *********/
Experience::Experience(double* state, double* resultingState, int stateLength, double reward, int action) {
    this->state = (double*) malloc(sizeof(double) * stateLength);
    memcpy(this->state, state, sizeof(double) * stateLength);

    this->resultingState = (double*) malloc(sizeof(double) * stateLength);
    memcpy(this->resultingState, resultingState, sizeof(double) * stateLength);

    this->action = action;
    this->reward = reward;
}

Experience::~Experience() {
    free(state);
    free(resultingState);
}

double* Experience::getState() { return state; }
double* Experience::getResultingState() { return resultingState; }
double Experience::getReward() { return reward; }
int Experience::getAction() { return action; }

/********* Experience Table *********/
ExperienceTable::ExperienceTable() { }
ExperienceTable::~ExperienceTable() { }

Experience* ExperienceTable::addExperience(double* state, double* resultingState, int stateLength, double reward, int action) {
    experiences.push_back(new Experience(state, resultingState, stateLength, reward, action));

    return experiences.at(experiences.size() - 1);
}

Experience* ExperienceTable::getRandomExperience() {
    return experiences.at(rand() % experiences.size());
}

std::vector<Experience*>& ExperienceTable::getExperiences() { return experiences; }