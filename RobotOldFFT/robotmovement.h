#ifndef ROBOTMOVEMENT_H
#define ROBOTMOVEMENT_H
//---------------------------------------------------------------------------------------//
//brugt eksempel kode

#include <chrono>
#include <thread>
#include <csignal> // Catch sigterm (CTRL + C)
#include "mqtt/async_client.h"
#include "json.hpp"
#include <vector>

using namespace std;
using json = nlohmann::json;

//const string ADDRESS { "tcp://192.168.109.105:1883" }; Krispy netværk
const string ADDRESS { "tcp://localhost:1883" };
//const string ADDRESS { "tcp://192.168.176.105:1883" };
const string TOPIC { "cmd_vel" };
const int QOS = 1;



//---------------------------------------------------------------------------------------//

class RobotMovement
{
public:
    RobotMovement(mqtt::async_client& client, mqtt::topic& topic);
    ~RobotMovement();
    json Move(bool dir, double  speed);
    json Turn(bool dir, double turn);
    //void Disconnect();
    void publishMessage(json j);
    bool Connect();
    void UdregningMove(int afstand, bool retning);
    void UdregningTurn(int vinkel, bool retning);
    void sendMovement();
    void addMovement(int afstand, bool retning);
    void addTurn(int vinkel, bool retning);

private:
    bool IsConnect();
    bool conn = false;
    double speed = 0.2;
    double turnDir;
    double rest;
    bool retning;
    int dist;
    double restfart;
    int antalOmdrejninger = 1;
    vector<int> afstandVec;
    vector<int> vinkelVec;
    vector<bool> retningVec;
    vector<bool> HVvec;
    vector<int> koeVec;
    vector<string> koeVec2;


//-------------------------------
    mqtt::async_client* cli;
    mqtt::topic* top;
    mqtt::token_ptr tok;
//-------------------------------
};



#endif // ROBOTMOVEMENT_H
