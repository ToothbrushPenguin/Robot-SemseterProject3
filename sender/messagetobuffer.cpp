#include "messagetobuffer.h"

MessageToBuffer::MessageToBuffer()
{
    for(int n = 0; n < packetLength; n++ ){
        a.push_back(1000*sin(n*2*M_PI*697/samplerate));
        b.push_back(1000*sin(n*2*M_PI*770/samplerate));
        c.push_back(1000*sin(n*2*M_PI*852/samplerate));
        d.push_back(1000*sin(n*2*M_PI*941/samplerate));

        e.push_back(1000*sin(n*2*M_PI*1209/samplerate));
        f.push_back(1000*sin(n*2*M_PI*1336/samplerate));
        g.push_back(1000*sin(n*2*M_PI*1477/samplerate));
        h.push_back(1000*sin(n*2*M_PI*1633/samplerate));
    }
}

sf::SoundBuffer MessageToBuffer::convert( vector<char> message )
{

    vector<sf::Int16> f1Lf;
    vector<sf::Int16> f2Hf;

    for(unsigned int i = 0; i < message.size(); i++){
        switch(message[i])
        {
         case '1': case '2': case '3': case 'A': f1Lf =  a; break;
         case '4': case '5': case '6': case 'B': f1Lf =  b; break;
         case '7': case '8': case '9': case 'C': f1Lf =  c; break;
         case '*': case '0': case '#': case 'D': f1Lf =  d; break;
        }
        switch(message[i])
        {
         case '1': case '4': case '7': case '*': f2Hf =  e; break;
         case '2': case '5': case '8': case '0': f2Hf =  f; break;
         case '3': case '6': case '9': case '#': f2Hf =  g; break;
         case 'A': case 'B': case 'C': case 'D': f2Hf =  h; break;
        }

        for(int n = 0; n < packetLength; n++ ){
        samplesAll.push_back(f1Lf[n]+f2Hf[n]);
        }
    }


    buffer.loadFromSamples(&samplesAll[0], samplesAll.size(), 1, samplerate);

    return buffer;
}
