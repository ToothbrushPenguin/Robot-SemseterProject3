#include "msgbuffer.h"

using namespace std;

int main()
{
    sf::Sound sound;
    MsgBuffer buf;
    sf::SoundBuffer buffer = buf.convert({'*','d','#'});
    sound.setBuffer(buffer);
    sound.setVolume(50);
    //sound.setLoop(true);
    sound.play();

    while(sound.getStatus()==2){

    }
    return 0;
}

//record
//new tread databehanling
//wait(x)
//stoprecod
//set databehanling buffer = record buffer
