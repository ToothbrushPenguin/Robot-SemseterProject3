#include <iostream>
#include "robotmovement.h"

using namespace std;


//---------------------------------

// denne kodes placering skal måske ændres

// Declared globally as signal handler can't be part of the class..
mqtt::async_client* cli;
mqtt::topic* top;
mqtt::token_ptr tok;

// Handle CTRL + C and stop the robot
void signal_Handler(int signum) {
    json stop_msg = {{"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
    {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
    };
    cout << "CTRL + C pressed, exiting.." << endl;
    tok = top->publish(stop_msg.dump());
    tok->wait();
    exit(signum);
};
//----------------------------------

int main()
{
    signal(SIGINT, signal_Handler);
    cout << "Initializing for server '" << ADDRESS << "'..." << endl;
    mqtt::async_client cli(ADDRESS, "");
    mqtt::topic top(cli, TOPIC, QOS);

    RobotMovement ex(cli, top);
    bool connected = ex.Connect();
    cout << "Connected: " << connected << endl;

    while(true)
    {
    bool forward = true;
    double speed = 0.5;
    json move = ex.RobotMovement::Move(forward,speed);

    ex.publishMessage(move);
    }
}
