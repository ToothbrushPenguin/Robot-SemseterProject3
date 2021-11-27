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
    //vector<vector<char>> msg = {{'5','8','1','*','9','5','*','*','8','6','7','9','3','8','3','6','9','5','8','4','5','9','1','8','1','2','9','2','8','3','4','9','4','8','5','6','9','7','8','1','*','2','9','0','8','5','*','*','8','3','*','9','2','8','1','2','9','7','*','*','8','5','1','*','9','7','*','*','8','6','7','9','3','8','3','6','9','5','8','4','5','9','1','8','1','1','*','9','7','*','*','8','6','5','9','0','8','0','8'}};
    vector<vector<char>> msg = {{'c','c','6'}};
    sound.setVolume(100);

    for(int i = 0; i < (int)msg.size(); i++){

        vector<char> incoded = handler.seqIncoder(msg[i], seq.getPN());
        incoded = handler.crcIncoder(incoded);
        incoded = handler.ssbit(incoded);
        //vector<char> incoded = handler.ssbit(msg[0]);

        for(int j = 0; j < (int)incoded.size(); j++){
            cout << incoded.at(j);
        }
        cout << endl<<endl;
        cout << incoded.size() << endl;
        //vector<char> incoded = handler.ssbit(msg[i]);
        sf::SoundBuffer buffer;

        for(unsigned int u = 0; u< (incoded.size()); u++){//for(unsigned int u = 0; u< (incoded.size()*2); u++){
            cout << u << endl;
            //if(u%2==0){
              buffer = buf.convert({incoded[u]}); // buffer = buf.convert({incoded[u/2]});
            //}else{
            //  buffer = buf.convert({'S'});
            //}

            sound.setBuffer(buffer);
            //sound.setLoop(true);
            sound.play();
            while(sound.getStatus()==2){
            }
        }




        vector<char> robReply = bM.SignalRecord(500);

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
