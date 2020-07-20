#include <stdio.h>
#include <iostream>
#include <time.h>
#include <vector>

#include "LayeredNetwork/NetworkBuilder.hpp"
#include "LayeredNetwork/LayeredNetwork.hpp"
#include "Databases/MnistDatabase.hpp"
#include "Environments/TetrisEnvironment.hpp"
#include "ExperienceTables/ExperienceTable.hpp"

int main() {
    TetrisEnvironment environment;
    ExperienceTable experienceTable;

    NetworkBuilder builder;
    builder.inputRows(202);

    builder.fullyConnectedLayer(Momentum, 256);
    builder.activationLayer(Sigmoid);
    builder.fullyConnectedLayer(Momentum, 256);
    builder.activationLayer(Sigmoid);
    builder.fullyConnectedLayer(Momentum, 256);
    builder.activationLayer(Sigmoid);
    builder.fullyConnectedLayer(Momentum, 800);

    LayeredNetwork* outputNetwork = builder.build();

    outputNetwork->getNetworkInformation().setLearningRate(0.001);
    outputNetwork->getNetworkInformation().setVelocityCoefficient(0.9);
    outputNetwork->getNetworkInformation().setBatchSize(20);

    LayeredNetwork* targetNetwork = new LayeredNetwork(outputNetwork);
    targetNetwork->copyState(outputNetwork);

    double* currentState = (double*) malloc(sizeof(double) * 202);
    double* nextState = (double*) malloc(sizeof(double) * 202);

    double epsilon = 1.0;
    double discountFactor = 0.9;

    environment.initialState(currentState);

    //List of legal actions for choosing a random legal action
    std::vector<int> legalActions;
    for(int i = 0; i < 10000; i++) {
        Matrix* output = outputNetwork->feedForward(currentState);

        double greedyChoice = (double) rand() / RAND_MAX;
        bool greedy = greedyChoice <= epsilon;

        //Place holder for chosen action
        int action = -1;

        //Filter out illegal moves
        legalActions.clear();
        for(int i = 0; i < output->getLength(); i++) {
            bool legal = environment.isLegalAction(currentState, i);

            if(!legal) continue;

            if(greedy) {
                if(action == -1)
                    action = i;
                else if(output->getData()[i] > output->getData()[action])
                    action = i;
            } else {
                legalActions.push_back(i);
            }
        }

        //AI topped out and is bad...
        if(legalActions.size() == 0 && action == -1) {
            i--;
            environment.initialState(currentState);
            continue;
        }
        
        if(!greedy) {
            action = legalActions.at(rand() % legalActions.size());
        }

        //Preform action in "emulator" and store results in table
        double reward = environment.preformAction(currentState, nextState, action);

        experienceTable.addExperience(currentState, nextState, 202, reward, action);
    }

    for(int i = 0; i < 5000; i++) {
        if(i % 20 == 0)
            targetNetwork->copyState(outputNetwork);

        Experience* experience = experienceTable.getRandomExperience();

        Matrix* output = outputNetwork->feedForward(experience->getState());
        Matrix* targetOutput = targetNetwork->feedForward(experience->getResultingState());

        //Find the max (legal) Q Value for the resulting state
        double maxQValue;
        bool hasLegalAction = false;

        for(int i = 0; i < output->getLength(); i++) {
            bool legal = environment.isLegalAction(experience->getResultingState(), i);

            if(!legal) continue;

            if(!hasLegalAction) {
                hasLegalAction = true;
                maxQValue = targetOutput->getData()[i];
            } else if(targetOutput->getData()[i] > maxQValue) {
                maxQValue = targetOutput->getData()[i];
            }
        }

        double loss;
        if(hasLegalAction)
            loss = pow(experience->getReward() + (discountFactor * maxQValue) - targetOutput->getData()[experience->getAction()], 2);
        else
            loss = pow(experience->getReward() - targetOutput->getData()[experience->getAction()], 2);

        outputNetwork->addGradient(loss, experience->getAction());

        if(outputNetwork->getNetworkInformation().incrementBatchIndex())
            outputNetwork->update();
    }

    delete outputNetwork;
    delete targetNetwork;
}