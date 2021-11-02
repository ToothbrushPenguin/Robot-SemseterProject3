#include <iostream>
#include "buffermsg.h"
#include "msgbuffer.h"
#include "msghandeler.h"
#include "robotmovement.h"

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
    MsgHandeler handeler;
    vector<Direction> dirs;
    vector<double> vals;

    bool boolW = 1;

    while(boolW){
        vector<char> message = bM.SignalRecord();

        handeler.handshake(handeler.isValid(message),handeler.readPn(message));
        //while(!(handeler.isValid(message))){
        //   message = bM.SignalRecord();
        //   handeler.handshake(handeler.isValid(message),handeler.readPn(message));
        //}

        if(handeler.isStartStop(message) == RUNNING){
            dirs.push_back(handeler.DecodeMovement(message));
            vals.push_back(handeler.decodeValue(message));
        }else if (handeler.isStartStop(message)== STOP){
            boolW = 0;
        }
    }

}
