#ifndef SEQHANDLER_H
#define SEQHANDLER_H
#include "msgbuffer.h"

class SeqHandler
{
public:
    SeqHandler();
    bool validatePN(vector<char> msg);
    void incPN();
    int getPN();
    void resetPN();

private:
    int pn;
};

#endif // SEQHANDLER_H
