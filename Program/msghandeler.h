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

    void handshake(bool d);
    State isStartStop(vector<char> e);
    Direction DecodeMovement(vector<char> e);
    double decodeValue(vector<char> e);

    vector<char> incoder(vector<char> e);
    bool isValid(vector<char> e);

    vector<Direction> dirs;
};

#endif // MSGHANDELER_H
