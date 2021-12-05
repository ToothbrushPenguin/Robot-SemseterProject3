#include <iostream>
#include "../Classes/buffermsg.h"
#include "../Classes/msgbuffer.h"
#include "../Classes/msghandler.h"
#include "../Classes/robotmovement.h"

using namespace std;

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
    BufferMsg bM;
    MsgHandler handeler;
    vector<Direction> dirs;
    vector<double> vals;
    bool boolW = 1;

    //------------------------------------------------------------------------------
                       //krevet kode for at connect turtlebot

       signal(SIGINT, signal_Handler);
       cout << "Initializing for server '" << ADDRESS << "'..." << endl;
       mqtt::async_client cli(ADDRESS, "");
       mqtt::topic top(cli, TOPIC, QOS);

       RobotMovement ex(cli, top);
       bool connected = ex.Connect();
       cout << "Connected: " << connected << endl;

    //------------------------------------------------------------------------------
                      //besked modtagelse


    //------------------------------------------------------------------------------
                      //bevægelse af robotten
    cout << "time to drive" << endl;

    ex.addTurn(4, true);
    ex.addTurn(4, false);
    ex.addMovement(20, true);
    ex.addMovement(20, false);

    cout << "send" << endl;
    ex.sendMovement();

    //---------------------------------------------------------------------------
}
