#include <iostream>

#include <sys/types.h>
#include <sys/sysinfo.h>

#include "Web/SocketIO/sio_client.h"

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
        });

        // socket->on("get-models", [&](sio::event& event) {

        // });

        socket->on("sys-info", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp) {
            std::cout << "Got a sys-info request" << std::endl;

            sio::message::ptr m(sio::object_message::create());
            auto &map = m.get()->get_map();

            struct sysinfo memInfo;
            sysinfo(&memInfo);

            long long totalPhysicalRAM = memInfo.totalram;
            long long totalPhysicalRAMUsed = memInfo.totalram - memInfo.freeram;

            totalPhysicalRAM *= memInfo.mem_unit;
            totalPhysicalRAMUsed *= memInfo.mem_unit;

            map.insert(std::make_pair("Total RAM", sio::string_message::create(std::to_string(totalPhysicalRAM * 9.31E-10))));
            map.insert(std::make_pair("Used RAM", sio::string_message::create(std::to_string(totalPhysicalRAMUsed * 9.31E-10))));

            client.socket()->emit("sys-info-reply", m);
        }));

        socket->on("connection", [&](sio::event& event) {
            std::cout << "Connected" << std::endl;
        });
    });

    client.connect("http://127.0.0.1:3000");
}