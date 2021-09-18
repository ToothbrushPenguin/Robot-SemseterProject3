#include <iostream>
#include "messagetobuffer.h"

using namespace std;

int main()
{
    sf::Sound sound;
    MessageToBuffer buf;
    sf::SoundBuffer buffer = buf.convert({'#','3','7','3','A','D','3','B','3','*','0','A','D'});
    sound.setBuffer(buffer);
    sound.setVolume(100);
    sound.setLoop(true);
    sound.play();

    while(sound.getStatus()==2){

    }
    return 0;
}
