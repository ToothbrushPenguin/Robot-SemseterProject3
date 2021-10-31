#include "msghandeler.h"
#include "seqhandler.h"
#include "buffermsg.h"

using namespace std;

int main()
{
    sf::Sound sound;
    BufferMsg bM;
    MsgBuffer buf;
    MsgHandeler handler;
    SeqHandler seq;
    vector<vector<char>> msg = {{'a'}};
    //vector<vector<char>> msg = {{'*','a','a','1','0','#'},{'*','b','b','5','0','#'},{'*','a','a','2','0','#'}};
    sound.setVolume(100);


    for(unsigned int i = 0; i < msg.size(); i++){
        //vector<char> incoded = handler.seqIncoder(msg[i], seq.getPN());
        //incoded = handler.crcIncoder(incoded);
        //incoded = handler.ssbit(incoded);
        vector<char> incoded = handler.ssbit(msg[i]);

        sf::SoundBuffer buffer = buf.convert(incoded);
        sound.setBuffer(buffer);
        //sound.setLoop(true);
        sound.play();
        while(sound.getStatus()==2){
        }

        //vector<char> robReply = bM.SignalRecord(1000);
        //
        //if(handler.isValid(robReply)){
        //    if(!seq.validatePN(robReply)){
        //        i--;
        //    }
        //}else{
        //    i--;
        //}
    }
    return 0;
}
