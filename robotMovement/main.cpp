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
string fb;
string mr;
double speed;
double turnspeed;
int dist;

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
    bool direction = true;

    while(true)
    {

        speed = 0;
        turnspeed = 0;
        dist = 1;

        cout << "You want to move or rotate?: m,r" << endl;
        cin >> mr;
        if(mr == "m")
        {
            cout << "how fast you want to move? :" << endl;
            cin >> speed;

            cout << "how long you want to move? :" << endl;
            cin >> dist;

            cout << "Forward or backwords? f,b" << endl;
            cin >> fb;

            if(fb == "f")
            {
                forward = true;
            }else
            {
                forward = false;
            }

                movement = ex.RobotMovement::Move(forward,speed);
        }
        else
        {
            cout << "How fast you wanna turn ? :" << endl;
            cin >> turnspeed;
            if(turnspeed != 0)
            {
                cout << "Left or Right ? l,r" << endl;
                cin >> fb;

                if(fb == "l")
                {
                    direction = false;
                }
                else
                {
                    direction = true;
                }

                cout << "how far do you want to rotate? :" << endl;
                cin >> dist;

                movement = ex.RobotMovement::Turn(direction,turnspeed);

            }
        }


    //---------------------------------------------------------------------------

    //Sends json to turtlebot
        for(int i = 0; i < dist; i++)
        {
            ex.publishMessage(movement);
            this_thread::sleep_for(chrono::milliseconds(200));
        }


    //Sleeps for 2000 ms and clears the terminal.
    this_thread::sleep_for(chrono::milliseconds(2000));
    cout << "\033[2J\033[1;1H";

    }
}
