#ifndef MESSAGETOBUFFER_H
#define MESSAGETOBUFFER_H

#include <SFML/Audio.hpp>
#include <iostream>
#include <math.h>

using namespace std;

class MessageToBuffer
{
public:
    MessageToBuffer();
    sf::SoundBuffer convert(vector<char>message);


    sf::SoundBuffer buffer;
    vector<sf::Int16> samplesAll;

    unsigned int samplerate = 8000;
    int milisecondsPrBip = 100;
    int packetLength = milisecondsPrBip * samplerate/1000;

};

#endif // MESSAGETOBUFFER_H
