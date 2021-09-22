#ifndef BUFFERMSG_H
#define BUFFERMSG_H

#include "signalrecoder.h"

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <complex>
#include <iostream>
#include <chrono>
#include <thread>
#include <math.h>
#include <future>

using namespace std;
using namespace std::complex_literals;

class BufferMsg
{
public:
    BufferMsg();
    vector<char> SignalRecord();

private:
    vector<int> FourierSplit(vector<int> fsbuf);//retunere de 2 frekvencer
    char result(vector<int>);

    double samplerate = 8000;

    vector<sf::Int16> firstsampel = {0};
    sf::SoundBuffer fsbuf;

    SignalRecoder recorder;


    vector<int> fsout;


};

#endif // BUFFERMSG_H
