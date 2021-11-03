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
    //vector<vector<char>> msg = {{'a'}};
    vector<vector<char>> msg = {{'d'}};
    sound.setVolume(100);


    for(int i = 0; i < (int)msg.size(); i++){
        vector<char> incoded = handler.seqIncoder(msg[i], seq.getPN());
        incoded = handler.crcIncoder(incoded);
        incoded = handler.ssbit(incoded);

        for(int i = 0; i < (int)incoded.size(); i++){
            cout << incoded.at(i);
        }
        cout << endl<<endl;

        //vector<char> incoded = handler.ssbit(msg[i]);

        sf::SoundBuffer buffer = buf.convert(incoded);
        sound.setBuffer(buffer);
        //sound.setLoop(true);
        sound.play();
        while(sound.getStatus()==2){
        }

        vector<char> robReply = bM.SignalRecord(3000);

        if(handler.isValid(robReply)){
            if(!seq.validatePN(robReply)){
                i--;
            }
        }else{
            i--;
        }
    }
    return 0;
}
