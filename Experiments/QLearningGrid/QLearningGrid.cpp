#include <stdio.h>
#include <iostream>
#include <time.h>
#include <vector>

#include "LayeredNetwork/NetworkBuilder.hpp"
#include "LayeredNetwork/LayeredNetwork.hpp"
#include "Databases/MnistDatabase.hpp"
#include "Environments/GridEnvironment.hpp"
#include "ExperienceTables/ExperienceTable.hpp"

#define GRID_ROWS 2
#define GRID_COLS 4

#define MAX_MOVES 20
#define REPLAY_MEMORY_SIZE 600

#define NUM_TIMES 50
#define NUM_COLLECTION_FOR_TARGET_COPY 1
#define NUM_DATA_COLLECT 200
#define NUM_BATCHES_TRAIN 2

#define EPSILON_DECAY 0.7
#define EPSILON_CAP 0.1

#define DISCOUNT_FACTOR 0.7

void collectData(LayeredNetwork* outputNetwork, GridEnvironment& environment, ExperienceTable& experienceTable, std::vector<int>& legalActions, double* currentState, double epsilon) {
    for(int i = 0; i < NUM_DATA_COLLECT; i++) {
        Matrix* output = outputNetwork->feedForward(environment.getBoard()->getData());

        double greedyChoice = (double) rand() / RAND_MAX;
        bool greedy = greedyChoice > epsilon;

        //Place holder for chosen action
        int action = -1;

        //Filter out illegal moves
        legalActions.clear();
        for(int j = 0; j < output->getLength(); j++) {
            bool legal = environment.canMoveByDirectionIndex(j);

            if(!legal) continue;

            if(greedy) {
                if(action == -1)
                    action = j;
                else if(output->getData()[j] > output->getData()[action])
                    action = j;
            } else {
                legalActions.push_back(j);
            }
        }

        if(!greedy) {
            action = legalActions.at(rand() % legalActions.size());
        }

        //Preform action in "emulator" and store results in table
        memcpy(currentState, environment.getBoard()->getData(), sizeof(double) * GRID_ROWS * GRID_COLS);

        double reward = environment.moveByDirectionIndex(action);

        // if(reward == GOAL_REWARD)
        //     std::cout << "GATHERING DATA HIT GOAL!" << std::endl;

        experienceTable.addExperience(currentState, environment.getBoard()->getData(), GRID_ROWS * GRID_COLS, reward, action);

        if(experienceTable.getExperiences().size() > REPLAY_MEMORY_SIZE)
            experienceTable.getExperiences().erase(experienceTable.getExperiences().begin());

        // environment.getBoard()->print();
        // std::cout << "Action: " << action << std::endl;
        // std::cout << "Reward: " << reward << std::endl;
        // output->print();
        // std::cout << "-----" << std::endl;

        if(reward == TERMINATE_PUNISHMENT || reward == GOAL_REWARD) {
            environment.reset();
        }
    }
}

void trainBatch(GridEnvironment& environment, ExperienceTable& experienceTable, LayeredNetwork* outputNetwork, LayeredNetwork* targetNetwork) {
    for(int batch = 0; batch < NUM_BATCHES_TRAIN; batch++) {
        for(int i = 0; i < outputNetwork->getNetworkInformation().getBatchSize(); i++) {
            // Experience* experience = experienceTable.getExperiences().at(i);
            Experience* experience = experienceTable.getRandomExperience();

            Matrix* output = outputNetwork->feedForward(experience->getState());
            Matrix* targetOutput = targetNetwork->feedForward(experience->getResultingState());

            //Find the max (legal) Q Value for the resulting state
            double maxQValue;
            bool hasLegalAction = false;

            environment.getBoard()->setData(experience->getResultingState());
            
            for(int j = 0; j < output->getLength(); j++) {
                if(!environment.canMoveByDirectionIndex(j)) 
                    continue;

                if(!hasLegalAction) {
                    hasLegalAction = true;
                    maxQValue = targetOutput->getData()[j];
                } else if(targetOutput->getData()[j] > maxQValue) {
                    maxQValue = targetOutput->getData()[j];
                }
            }

            // std::cout << "TRAINING EXPERIENCE REWARD: " << experience->getReward() << std::endl;;

            double loss;
            if(hasLegalAction && experience->getReward() != TERMINATE_PUNISHMENT && experience->getReward() != GOAL_REWARD)//Non-terminal
                loss = output->getData()[experience->getAction()] - experience->getReward() - (DISCOUNT_FACTOR * maxQValue);
            else//Terminal 
                loss =  output->getData()[experience->getAction()] - experience->getReward();

            outputNetwork->addGradient(loss, experience->getAction());

            if(outputNetwork->getNetworkInformation().incrementBatchIndex())
                outputNetwork->update();
        }
    }
}

double evaluation(GridEnvironment& environment, LayeredNetwork* outputNetwork) {
    double totalRewards = 0;

    environment.reset();

    for(int move = 0; move < MAX_MOVES; move++) {
        Matrix* output = outputNetwork->feedForward(environment.getBoard()->getData());

        //Place holder for chosen action
        int action = -1;

        //Filter out illegal moves
        for(int i = 0; i < output->getLength(); i++) {
            bool legal = environment.canMoveByDirectionIndex(i);

            if(!legal) continue;

            if(action == -1)
                action = i;
            else if(output->getData()[i] > output->getData()[action])
                action = i;
        }

        //Preform action in "emulator" and store results in table
        double reward = environment.moveByDirectionIndex(action);

        // environment.getBoard()->print();
        // std::cout << "Action: " << action << std::endl;
        // std::cout << "Reward: " << reward << std::endl;
        // output->print();
        // std::cout << "-----" << std::endl;

        totalRewards += reward;

        if(reward == TERMINATE_PUNISHMENT || reward == GOAL_REWARD) {
            environment.reset();
            break;
        }
    }

    return totalRewards;
}

int main() {
    srand(1);

    //For wandering
    GridEnvironment environment(GRID_ROWS, GRID_COLS);

    //For random sampling
    GridEnvironment testEnvironment(GRID_ROWS, GRID_COLS);

    ExperienceTable experienceTable;

    NetworkBuilder builder;
    builder.inputRows(GRID_ROWS * GRID_COLS);

    builder.fullyConnectedLayer(Momentum, 32);
    builder.activationLayer(Sigmoid);
    builder.fullyConnectedLayer(Momentum, 32);
    builder.activationLayer(Sigmoid);
    builder.fullyConnectedLayer(Momentum, 4);
    builder.activationLayer(Sigmoid);

    LayeredNetwork* outputNetwork = builder.build();

    outputNetwork->getNetworkInformation().setLearningRate(0.001);
    outputNetwork->getNetworkInformation().setVelocityCoefficient(0.9);
    outputNetwork->getNetworkInformation().setBatchSize(50);

    LayeredNetwork* targetNetwork = new LayeredNetwork(outputNetwork);
    targetNetwork->copyState(outputNetwork);

    double* currentState = (double*) malloc(sizeof(double) * GRID_ROWS * GRID_COLS);

    double epsilon = 1.0;

    for(int i = 0; i < 10; i++) {
        double eval = evaluation(environment, outputNetwork);
        std::cout << "Starting Total Rewards #" << i << ": " << eval << std::endl;
    }

    std::cout << "------" << std::endl;

    /******* TRAINING *******/

    //List of legal actions for choosing a random legal action
    std::vector<int> legalActions;
    for(int i = 0; i < NUM_TIMES; i++) {
        targetNetwork->copyState(outputNetwork);

        collectData(outputNetwork, environment, experienceTable, legalActions, currentState, epsilon);
        trainBatch(testEnvironment, experienceTable, outputNetwork, targetNetwork);

        epsilon -= EPSILON_DECAY;

        if(epsilon < EPSILON_CAP)
            epsilon = EPSILON_CAP;
    }

    for(int i = 0; i < 10; i++) {
        double eval = evaluation(environment, outputNetwork);
        std::cout << "Ending Total Rewards #" << i << ": " << eval << std::endl;
    }

    // double eval2 = evaluation(environment, outputNetwork);
    // std::cout << "Ending Total Rewards #" << eval2 << std::endl;

    delete outputNetwork;
    delete targetNetwork;
}