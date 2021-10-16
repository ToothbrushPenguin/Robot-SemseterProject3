#ifndef MSGHANDELER_H
#define MSGHANDELER_H
#include <vector>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;

enum Direction
{
    LEFT, RIGHT, UP, DOWN, STOP
};

class MsgHandeler
{
public:
    MsgHandeler();

    void handshake(bool);
    void isStartStop(vector<char> e);
    vector<Direction> DecodeMovement(vector<char> e);
    double decodeValue(vector<char> e);

    vector<char> incoder(vector<char> e);
    bool isValid(vector<char> e);

    bool startStop = false;
    vector<Direction> dirs;
};

#endif // MSGHANDELER_H
