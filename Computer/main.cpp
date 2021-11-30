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
    vector<vector<char>> msg = {{'a','1'}};
    sound.setVolume(100);

    for(int i = 0; i < (int)msg.size(); i++){

        vector<char> incoded = handler.seqIncoder(msg[i], seq.getPN());
        incoded = handler.crcIncoder(incoded);
        incoded = handler.ssbit(incoded);

        for(int j = 0; j < (int)incoded.size(); j++){
            cout << incoded.at(j);
        }
        cout << endl<<endl;
        cout <<incoded.size() << endl;
        sf::SoundBuffer buffer;

        for(unsigned int u = 0; u< (incoded.size()); u++){
            cout << u << endl;
            buffer = buf.convert(incoded[u]);

            sound.setBuffer(buffer);
            sound.play();
            while(sound.getStatus()==2){
            }
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
