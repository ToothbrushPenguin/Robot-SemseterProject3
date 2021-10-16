#include <iostream>
#include "buffermsg.h"
#include "msgbuffer.h"
#include "msghandeler.h"

using namespace std;

int main()
{
    BufferMsg bM;
    MsgHandeler handeler;
    vector<Direction> dirs;
    vector<double> vals;
    bool boolW = 1;
    while(boolW){
        vector<char> message = bM.SignalRecord();

        handeler.handshake(handeler.isValid(message));
        while(!(handeler.isValid(message))){
           message = bM.SignalRecord();
           handeler.handshake(handeler.isValid(message));
        }

        if(handeler.isStartStop(message) == RUNNING){
            dirs.push_back(handeler.DecodeMovement(message));
            vals.push_back(handeler.decodeValue(message));
        }else if (handeler.isStartStop(message)== STOP){
            boolW = 0;
        }
    }

}
