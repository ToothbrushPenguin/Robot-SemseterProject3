#include "msgbuffer.h"

MsgBuffer::MsgBuffer()
{
    for(int n = 0; n < packetLength; n++ ){
        a.push_back(10000*sin(n*2*M_PI*697/samplerate));
        b.push_back(10000*sin(n*2*M_PI*770/samplerate));
        c.push_back(10000*sin(n*2*M_PI*852/samplerate));
        d.push_back(10000*sin(n*2*M_PI*941/samplerate));

        e.push_back(10000*sin(n*2*M_PI*1209/samplerate));
        f.push_back(10000*sin(n*2*M_PI*1336/samplerate));
        g.push_back(10000*sin(n*2*M_PI*1477/samplerate));
        h.push_back(10000*sin(n*2*M_PI*1633/samplerate));
    }
}

vector<sf::Int16> MsgBuffer::winFunc(vector<sf::Int16> samp)
{
    vector<sf::Int16> nsamp={};
    int n=samp.size();

    for(int i = 0; i < (int)samp.size();i++){
    nsamp.push_back(samp.at(i)*(0.5*(1-cos(2*M_PI*i/(n-1)))));
    }

    return nsamp;
}

sf::SoundBuffer MsgBuffer::convert(char message )
{

    vector<sf::Int16> f1Lf;
    vector<sf::Int16> f2Hf;
    vector<sf::Int16> samplesAll;


    switch(message){
        case '1': case '2': case '3': case 'a': f1Lf =  a; break;
        case '4': case '5': case '6': case 'b': f1Lf =  b; break;
        case '7': case '8': case '9': case 'c': f1Lf =  c; break;
        case '*': case '0': case '#': case 'd': f1Lf =  d; break;
    }
    switch(message){
        case '1': case '4': case '7': case '*': f2Hf =  e; break;
        case '2': case '5': case '8': case '0': f2Hf =  f; break;
        case '3': case '6': case '9': case '#': f2Hf =  g; break;
        case 'a': case 'b': case 'c': case 'd': f2Hf =  h; break;
    }

    for(int n = 0; n < packetLength; n++ ){
        samplesAll.push_back(f1Lf[n]+f2Hf[n]);
        }    
    samplesAll=winFunc(samplesAll);

    sf::SoundBuffer buffer;
    buffer.loadFromSamples(&samplesAll[0], (int)samplesAll.size(), 1, samplerate);

    return buffer;
}
