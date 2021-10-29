#include "seqhandler.h"

SeqHandler::SeqHandler()
{

}

bool SeqHandler::validatePN(vector<char> msg)
{
    SeqHandler seq;
    for(unsigned int i = 1; i < msg.size()-5; i++){
        if(msg[i-1] == 'a' && msg [i] == 'b'){
            int robPn = ((int)msg[i+1]-48);
            if(robPn == pn){
                seq.incPN();
                return true;
            }
        }
    }
    return false;
}

void SeqHandler::incPN()
{
    if(pn > 8){
        resetPN();
    }else{
        pn++;
    }
}

int SeqHandler::getPN()
{
    return pn;
}

void SeqHandler::resetPN()
{
    pn = 1;
}
