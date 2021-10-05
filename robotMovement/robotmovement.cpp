#include "robotmovement.h"

using namespace std;

RobotMovement::RobotMovement(mqtt::async_client& client, mqtt::topic& topic)
{
    cli = &client;
    top = &topic;
}

RobotMovement::~RobotMovement()
{

}

json RobotMovement::Move(bool dir, double dist)
{
    if(dir == true)
    {
        dist = -dist;
    }

    json j =
        {
        {"linear", {{"x", dist}, {"y", 0}, {"z", 0}}},
        {"angular", {{"x", 0}, {"y", 0}, {"z", 0}}}
        };

//Vi skal have testet forskellige hastigheder.
//1. For at kunne finde en sikker hastighed der ikke går får hurtigt.
//2. For at kunne beregne hvor lang tid den skal køre med den hastighed for at nå en hvis længde.
//dir definere om det er forlæns eller bagud, 0 == bagud, 1 == fremad.
    return j;
}
json RobotMovement::Turn(bool dir, double turn)
{
    if(dir == false)
    {
        turn = -turn;
    }

    json j =
        {
        {"linear", {{"x", 0}, {"y", 0}, {"z", 0}}},
        {"angular", {{"x", 0}, {"y", 0}, {"z", turn}}}
        };

//Vi får dir, som skal definere om det er mod højre eller venstre.
//0 == venstre, 1 == Højre
//Herefter turn, som definere hvor mange grader, i radianer, den skal dreje mod den specificerede side
    return j;
}

void RobotMovement::publishMessage(json j)
{
    try {
        tok = top->publish(j.dump());
        tok->wait();
    }
    catch (const mqtt::exception& exc) {
        cerr << exc << endl;
        return;
    }
}

bool RobotMovement::Connect() // Connecter robotten og outputter i terminal
{
    try {
        cout << "\nConnecting..." << endl;
        cli->connect()->wait();
        cout << "  ...OK" << endl;
    }
    catch (const mqtt::exception& exc) {
        cerr << exc << endl;
        return false;
    }
    return true;
}

bool RobotMovement::IsConnect()
{
return conn;
}
