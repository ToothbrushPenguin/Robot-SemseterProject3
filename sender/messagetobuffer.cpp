#include "messagetobuffer.h"

MessageToBuffer::MessageToBuffer()
{

}

sf::SoundBuffer MessageToBuffer::convert( vector<char> message )
{

    double f1Lf;
    double f2Hf;

    for(unsigned int i = 0; i < message.size(); i++){
        switch(message[i])
        {
         case '1': case '2': case '3': case 'A': f1Lf =  697; break;
         case '4': case '5': case '6': case 'B': f1Lf =  770; break;
         case '7': case '8': case '9': case 'C': f1Lf =  852; break;
         case '*': case '0': case '#': case 'D': f1Lf =  941; break;
        }
        switch(message[i])
        {
         case '1': case '4': case '7': case '*': f2Hf =  1209; break;
         case '2': case '5': case '8': case '0': f2Hf =  1336; break;
         case '3': case '6': case '9': case '#': f2Hf =  1477; break;
         case 'A': case 'B': case 'C': case 'D': f2Hf =  1633; break;
        }

        for(int n = 0; n < packetLength; n++ ){
        samplesAll.push_back(1000*sin(n*2*M_PI*f1Lf/samplerate)+1000*sin(n*2*M_PI*f2Hf/samplerate));
        }
    }


    buffer.loadFromSamples(&samplesAll[0], samplesAll.size(), 1, samplerate);

    return buffer;
}
