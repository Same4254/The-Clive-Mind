#ifndef EXPERIENCE_TABLE_H
#define EXPERIENCE_TABLE_H

#include <vector>
#include <memory>

class Experience {
    double* state;
    double reward;
    unsigned int action;
    double* resState;    
};

class ExperienceTable {
public:
    std::vector<std::unique_ptr<Experience>> experiences;

    ExperienceTable();
    virtual ~ExperienceTable();

    Experience* addExperience();
};
#endif