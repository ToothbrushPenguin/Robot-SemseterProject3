#include <iostream>
#include "robotmovement.h"
#include <vector>

using namespace std;


//---------------------------------

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


int main()
{

//------------------------------------------------------------------------------
                   //krevet kode for at connect turtlebot

    signal(SIGINT, signal_Handler);
    cout << "Initializing for server '" << ADDRESS << "'..." << endl;
    mqtt::async_client cli(ADDRESS, "");
    mqtt::topic top(cli, TOPIC, QOS);

    RobotMovement ex(cli, top);
    bool connected = ex.Connect();
    cout << "Connected: " << connected << endl;

//---------------------------------------------------------------------------

                          //Gøgl jeg har lavet til testning
    bool forward = true;
    bool backward = false;
    bool right = true;
    bool left = false;


    ex.addMovement(300, forward);
    ex.addTurn(2, left);
    ex.addMovement(200, forward);
    ex.addTurn(2, right);
    ex.addMovement(300, forward);
    ex.addTurn(8, right);
    ex.sendMovement();

}
