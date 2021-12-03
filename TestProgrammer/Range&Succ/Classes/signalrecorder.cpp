#include "signalrecorder.h"

SignalRecorder::SignalRecorder()
{

}
bool SignalRecorder::onStart()
{
    // Initialize whatever has to be done before the capture starts
    setProcessingInterval(sf::milliseconds(25));

    // Return true to start playing
    return true;
}
bool SignalRecorder::onProcessSamples(const int16_t *samples, size_t sampleCount)
{
    const lock_guard<mutex> lock(mutexlock);
    for(unsigned int j = 0; j < sampleCount; j++){
        samp.push_back(samples[j]);
    }
    return 1;
}

void SignalRecorder::onStop()
{

}

vector<int> SignalRecorder::getSamp()
{
    const lock_guard<mutex> lock(mutexlock);
    tempsamp = samp;
    samp.clear();
    return tempsamp;
}
