#include "seqhandler.h"

SeqHandler::SeqHandler()
{

}

bool SeqHandler::validatePN(vector<char> msg)
{
    SeqHandler seq;

    int robPn = ((int)msg[msg.size()-6]-48);
    if(robPn == pn){
        seq.incPN();
        return true;
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
