#include "seqhandler.h"

SeqHandler::SeqHandler()
{

}

bool SeqHandler::validatePN(vector<char> msg)
{
    for(unsigned int i = 1; i < msg.size()-5; i++){
        if(msg[i-1] == 'a' && msg [i] == 'b'){
            int robPn = ((int)msg[i+1]-48)*10+((int)msg[i+2]-48);
            if(robPn == pn){
                pn++;
                return true;
            }
        }
    }
    return false;
}

void SeqHandler::incPN()
{
    pn++;
}

int SeqHandler::getPN()
{
    return pn;
}

void SeqHandler::resetPN()
{
    pn = 1;
}
