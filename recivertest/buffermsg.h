#ifndef BUFFERMSG_H
#define BUFFERMSG_H

#include "signalrecoder.h"

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <complex>
#include <iostream>
#include <chrono>
//#include <thread>
#include <math.h>
#include <future>

using namespace std;
using namespace std::complex_literals;

class BufferMsg
{
public:
    BufferMsg();
    vector<char> SignalRecord();
    ~BufferMsg();

private:
    vector<int> FourierSplit(vector<int> fsbuf);//retunere de 2 frekvencer
    vector<int> twoLargest(vector<double>chancein);
    char result(vector<int>);

    double samplerate = 8000;
    vector<double> frequencys = {697,770,852,941,1209,1336,1477,1633};
    vector<sf::Int16> firstsampel = {0};
    sf::SoundBuffer fsbuf;

    SignalRecoder recorder;


    vector<int> fsout;


};

#endif // BUFFERMSG_H
