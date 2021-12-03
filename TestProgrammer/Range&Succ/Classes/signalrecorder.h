#ifndef SIGNALRECORDER_H
#define SIGNALRECORDER_H

#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <complex>
#include <iostream>
#include <chrono>
#include <math.h>
#include <future>

using namespace std;
using namespace sf;
using namespace std::complex_literals;

class SignalRecorder  : public SoundRecorder
{
public:
    SignalRecorder();

    virtual bool onStart() override;
    virtual bool onProcessSamples(const int16_t* samples, size_t sampleCount) override;
    virtual void onStop() override;

    vector<int> getSamp();

    mutex mutexlock;

    vector<int> samp={0};
    vector<int> tempsamp;
};

#endif // SIGNALRECORDER_H
