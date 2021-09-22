#include "signalrecoder.h"

SignalRecoder::SignalRecoder()
{

}

bool SignalRecoder::onStart()
{
    // Initialize whatever has to be done before the capture starts
    setProcessingInterval(sf::milliseconds(100));

    // Return true to start playing
    return true;
}
bool SignalRecoder::onProcessSamples(const int16_t *samples, size_t sampleCount)
{
    i++;
    samp.clear();
    for(unsigned int j = 0; j < sampleCount; j++){
        samp.push_back(samples[j]);
    }
    //cout << "i work" << i<< endl;
    return 1;
}

void SignalRecoder::onStop()
{

}

vector<int> SignalRecoder::getSampels()
{
    return samp;
}

