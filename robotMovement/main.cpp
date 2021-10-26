#include <iostream>
#include "robotmovement.h"
#include <cmath>

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
int afstand;
int dist;
double rest;
double hastighed;
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

        //dist = (afstand - 2.6553)/3.2885;



        dist = afstand / 4.775;

        rest = (afstand / 4.775)-dist;

        x = (rest - 1.55)/23.1;

        if(x < 0)
        {
            x = -x;
        }
        //231*x + 1,55

        movement = ex.Move(forward,0.2);


        cout << "dist: " << dist << endl;
        cout << "rest: " << rest << endl;
        cout << "x:" << x << endl;
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
