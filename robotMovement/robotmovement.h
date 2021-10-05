#ifndef ROBOTMOVEMENT_H
#define ROBOTMOVEMENT_H
//---------------------------------------------------------------------------------------//
//brugt eksempel kode

#include <chrono>
#include <thread>
#include <csignal> // Catch sigterm (CTRL + C)
#include "mqtt/async_client.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

//const string ADDRESS { "tcp://192.168.0.6:1883" };
const string ADDRESS { "tcp://localhost:1883" };
const string TOPIC { "cmd_vel" };
const int QOS = 1;

// Declared globally as signal handler can't be part of the class..
mqtt::async_client* cli;
mqtt::topic* top;
mqtt::token_ptr tok;

// Handle CTRL + C and stop the robot
void signalHandler(int signum) {
    json stop_msg = {{"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
    {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
    };
    cout << "CTRL + C pressed, exiting.." << endl;
    tok = top->publish(stop_msg.dump());
    tok->wait();
    exit(signum);
}
//---------------------------------------------------------------------------------------//

class RobotMovement
{
public:
    RobotMovement(mqtt::async_client& client, mqtt::topic& topic);
    ~RobotMovement();
    json Move(bool dir, double  dist);
    json Turn(bool dir, double turn);
    //void Disconnect();
    void publishMessage(json j);
    bool Connect();


private:
    bool IsConnect();
    bool conn = false;
    double speed;
    double turnDir;
};

#endif // ROBOTMOVEMENT_H
