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
    vector<char> decodeAndRun(vector<char> e);


    Direction DecodeMovement(vector<char>);
    double decodeValue(vector<char>);

    vector<char> incoder(vector<char>);
    bool isValid(vector<char>);

};

#endif // MSGHANDELER_H
