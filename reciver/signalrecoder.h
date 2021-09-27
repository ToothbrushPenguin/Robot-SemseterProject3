#ifndef SIGNALRECODER_H
#define SIGNALRECODER_H

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <complex>
#include <iostream>
#include <chrono>
//#include <thread>
#include <math.h>
#include <future>

using namespace std;
using namespace sf;

class SignalRecoder : public SoundRecorder
{
public:
    SignalRecoder();

    bool onStart() override;
    bool onProcessSamples(const int16_t* samples, size_t sampleCount) override;
    void onStop() override;

    vector<int> getSampels();


    vector<int> samp;

    int i=0;
};

#endif // SIGNALRECODER_H
