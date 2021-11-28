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
#include <fstream>
#include <numeric>

using namespace std;
using namespace std::complex_literals;


class BufferMsg
{
public:
    BufferMsg();
    vector<char> SignalRecord(int timeout = -1);
    ~BufferMsg();

    void getStats();//SKAL FJERNES STATS`
private:
    vector<int> FourierSplit(vector<int> fsbuf);//retunere de 2 frekvencer
    vector<int> twoLargest(vector<double>chancein);
    char result(vector<int>);
    vector<complex<double>> FastFourier(vector<complex<double>> fsbuf);
    vector<complex<double>> DFT(vector<complex<double>> sampleIn);
    int LargestInList(vector<double>list);
    vector<int> triWinFunk(vector<int> samp);
    vector<char> dumb(vector<char>list);

    double samplerate = 8192;
    vector<sf::Int16> firstsampel = {0};
    sf::SoundBuffer fsbuf;

    SignalRecoder recorder;

    int oldrecLength = 0;
    vector<int> rec={};
    vector<int> fsout;

    //skal fjernes
    ofstream samplefile;

    double succ = 0;
    double fail = 0;
    vector<char> succes ={'a','1','1','5','9','6','1','4'};

};

#endif // BUFFERMSG_H
