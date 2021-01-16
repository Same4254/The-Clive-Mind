#include<iostream>

#include "Web/SocketIO/sio_client.h"

void create(sio::message::list &data) {

}

/*
 * C++
 *  - Assign tasks to different PCs in the cluster
 *  - Pause and rseume tasks
 *  - change hyper parameters and learning methods (branching)
 *  - Combination Neural Networks
 *  - Read state file to get name, error, dataset, structure, and what state it was trained from
 *  - Run data sent from client (they write numbers in a canvas and the network tries to guess what it is)
 * 
 * Tasks:
 *  - Create model with a given structure
 *  - Model created with this structure
 *  
 *  - Train model on this data
 *  - Model trained on this data
 *  
 *  - View data from dataset
 *  
 *  - CPU and Memory usuage
 *  
 *  - pause and resume
 *  
 *  - change parameters/learning methods
 * 
 *  - combine neural networks
 * 
 *  - initialization query to ask what states are there for a given dataset
 *  
 *  - Run data through network
 */
int main() {
    sio::client client;

    client.set_open_listener([&]() {
        sio::socket::ptr socket = client.socket();

        socket->on("create", [&](sio::event& event) {
            std::cout << "Got a create messege" << std::endl;

            // auto dataMap = event.get_message()->get

            
        });

        socket->on("connection", [&](sio::event& event) {
            std::cout << "Connected" << std::endl;
        });
    });

    client.connect("http://127.0.0.1:3000");
}