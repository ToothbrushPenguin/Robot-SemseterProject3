#ifndef MSGHANDELER_H
#define MSGHANDELER_H
#include <vector>
#include <iostream>
#include <sstream>
#include <math.h>
#include "msgbuffer.h"

using namespace std;

enum Direction
{
    LEFT, RIGHT, UP, DOWN, HALT
};

enum State
{
    STOP, RUNNING
};

class MsgHandeler
{
public:
    MsgHandeler();

    void handshake(bool crc, int readPn);
    State isStop(vector<char> e);
    Direction DecodeMovement(vector<char> e);
    double decodeValue(vector<char> e);

    vector<char> crcIncoder(vector<char> e);
    vector<char> seqIncoder(vector<char> msg, int pnIn);
    vector<char> ssbit(vector<char> msg);
    bool isValid(vector<char> e);

    vector<Direction> dirs;
    int readPn(vector<char>msg);
    bool correctPn(int readpn);
    void resetRobPn();
    void incRobPn();
    int getRobPn();

private:
    int robPn = 0;
};

#endif // MSGHANDELER_H
