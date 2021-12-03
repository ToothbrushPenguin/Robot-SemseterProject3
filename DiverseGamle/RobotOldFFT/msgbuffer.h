#ifndef MSGBUFFER_H
#define MSGBUFFER_H

#include <SFML/Audio.hpp>
#include <iostream>
#include <math.h>

using namespace std;

class MsgBuffer
{
public:
    MsgBuffer();
    sf::SoundBuffer convert(vector<char>message);


    sf::SoundBuffer buffer;
    vector<sf::Int16> samplesAll;

    vector<sf::Int16> a;
    vector<sf::Int16> b;
    vector<sf::Int16> c;
    vector<sf::Int16> d;

    vector<sf::Int16> e;
    vector<sf::Int16> f;
    vector<sf::Int16> g;
    vector<sf::Int16> h;

    unsigned int samplerate = 8192;
    int milisecondsPrBip = 25;
    int packetLength = milisecondsPrBip * samplerate/1000;

};

#endif // MSGBUFFER_H
