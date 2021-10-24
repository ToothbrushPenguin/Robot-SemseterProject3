#include <iostream>
#include "robotmovement.h"

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
//----------------------------------
//variables
double afstand;
int dist;
double x;

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
    json movement;
    bool forward = true;

    while(true)
    {
        cout << "indtast afstand:";
        cin >> afstand;

        dist = (afstand - 2.6553)/3.2885;
        cout << "dist: " << dist << endl;
        x = (afstand-(dist*6.5))/23.3;

        cout << "x:" << x << endl;;
        movement = ex.Move(forward,0.13);

    //---------------------------------------------------------------------------

    //Sends json to turtlebot
        for(int i = 0; i < dist; i++)
        {
            ex.publishMessage(movement);
            this_thread::sleep_for(chrono::milliseconds(200));
        }

        movement = ex.Move(forward,x);

        ex.publishMessage(movement);
        this_thread::sleep_for(chrono::milliseconds(200));

    //Clears the terminal.
    //cout << "\033[2J\033[1;1H";

    }
}
