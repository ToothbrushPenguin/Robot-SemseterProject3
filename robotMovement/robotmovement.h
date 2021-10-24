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

//const string ADDRESS { "tcp://192.168.109.105:1883" }; Krispy netværk
//const string ADDRESS { "tcp://localhost:1883" };
const string ADDRESS { "tcp://192.168.176.105:1883" };
const string TOPIC { "cmd_vel" };
const int QOS = 1;



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
//-------------------------------
    mqtt::async_client* cli;
    mqtt::topic* top;
    mqtt::token_ptr tok;
//-------------------------------
};



#endif // ROBOTMOVEMENT_H
