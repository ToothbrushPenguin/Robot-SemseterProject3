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

        zero.push_back(5000*sin(n*2*M_PI*300/samplerate));
    }
}

sf::SoundBuffer MsgBuffer::convert( vector<char> message )
{

    vector<sf::Int16> f1Lf;
    vector<sf::Int16> f2Hf;
    vector<sf::Int16> samplesAll;
    for(unsigned int i = 0; i < message.size(); i++){
        if(message[0]=='S'){
            f1Lf = zero;
            f2Hf = zero;

        }else{
            switch(message[i])
            {
             case '1': case '2': case '3': case 'a': f1Lf =  a; break;
             case '4': case '5': case '6': case 'b': f1Lf =  b; break;
             case '7': case '8': case '9': case 'c': f1Lf =  c; break;
             case '*': case '0': case '#': case 'd': f1Lf =  d; break;
            }
            switch(message[i])
            {
             case '1': case '4': case '7': case '*': f2Hf =  e; break;
             case '2': case '5': case '8': case '0': f2Hf =  f; break;
             case '3': case '6': case '9': case '#': f2Hf =  g; break;
             case 'a': case 'b': case 'c': case 'd': f2Hf =  h; break;
            }

        }

        for(int n = 0; n < packetLength; n++ ){
        samplesAll.push_back(f1Lf[n]+f2Hf[n]);
        }    

    }

    sf::SoundBuffer buffer;
    buffer.loadFromSamples(&samplesAll[0], (int)samplesAll.size(), 1, samplerate);

    return buffer;
}
