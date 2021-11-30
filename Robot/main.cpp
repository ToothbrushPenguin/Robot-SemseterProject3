#include <iostream>
#include "buffermsg.h"
#include "msgbuffer.h"
#include "msghandeler.h"
#include "robotmovement.h"

using namespace std;

// Declared globally as signal handler can't be part of the class..
//mqtt::async_client* cli;
//mqtt::topic* top;
//mqtt::token_ptr tok;

// Handle CTRL + C and stop the robot
//void signal_Handler(int signum) {
//    json stop_msg = {{"linear", {{"x", 0.0}, {"y", 0}, {"z", 0}}},
//    {"angular", {{"x", 0}, {"y", 0}, {"z", 0.0}}}
//    };
//    cout << "CTRL + C pressed, exiting.." << endl;
//    tok = top->publish(stop_msg.dump());
//    tok->wait();
//    exit(signum);
//};

int main()
{
    BufferMsg bM;
    MsgHandeler handeler;
    vector<Direction> dirs;
    vector<double> vals;
    bool boolW = 1;

    //------------------------------------------------------------------------------
                       //krevet kode for at connect turtlebot

    //   signal(SIGINT, signal_Handler);
    //   cout << "Initializing for server '" << ADDRESS << "'..." << endl;
    //   mqtt::async_client cli(ADDRESS, "");
    //   mqtt::topic top(cli, TOPIC, QOS);
    //
    //   RobotMovement ex(cli, top);
    //   bool connected = ex.Connect();
    //   cout << "Connected: " << connected << endl;

    //------------------------------------------------------------------------------
                      //besked modtagelse

    //while(boolW){
    vector<char> message;

        message = bM.SignalRecord();

        handeler.handshake(handeler.isValid(message),handeler.readPn(message));
        //------for test------

        while(!(handeler.isValid(message))){
           message = bM.SignalRecord(3000);
           handeler.handshake(handeler.isValid(message),handeler.readPn(message));
        }
        //------------

        //cout << handeler.decodeValue(message) << endl;

        if(handeler.isStop(message) == RUNNING){
            dirs.push_back(handeler.DecodeMovement(message));
            vals.push_back(handeler.decodeValue(message));
        }else if (handeler.isStop(message)== STOP){
            boolW = 0;
        }


    //}

    //for(unsigned int i = 0; i < dirs.size(); i++){
    //    cout << dirs.at(i) << " : " << vals.at(i) << endl;
    //}

    //------------------------------------------------------------------------------
                      //bevægelse af robotten
    //for(unsigned int i = 0; i < dirs.size(); i++){
    //    switch (dirs.at(i)){
    //    case LEFT:
    //        ex.addTurn(vals.at(i), 1);
    //        break;
    //    case RIGHT:
    //        ex.addTurn(vals.at(i), 0);
    //        break;
    //    case UP:
    //        ex.addMovement(vals.at(i), 1);
    //        break;
    //    case DOWN:
    //        ex.addMovement(vals.at(i), 0);
    //        break;
    //    case HALT:
    //        ex.addMovement(0, 1);
    //        break;
    //    default:
    //        break;
    //    }
    //}
    //ex.sendMovement();

    //---------------------------------------------------------------------------
}
