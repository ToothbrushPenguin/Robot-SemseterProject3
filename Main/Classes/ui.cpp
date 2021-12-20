#include "ui.h"

Ui::Ui()
{

}

void Ui::runUi()
{
    vector<vector<char>> msg ={};
    vector<vector<char>> showMsg = {};
    char play = 'y';
    char tplay;
    char move;
    string tmove;
    string distance;
    int msgNr = 0;
    int moves =0;
    int delIndex;

    while (play=='y'){
        cout << "Do you wish to begin a route? [y/n] "<<endl;
        cin >> tplay;
        if(tplay=='y'){
            play = tplay;
            msg = {};
            moves = 1;
            for(int i=0;i<moves;i++){
                if(msgNr!=0){
                    cout <<endl<< "Current route: "<<endl;

                    for (uint k=0;k<showMsg.size();k++){
                        for(uint l=0;l<showMsg[k].size();l++){
                            cout << showMsg[k][l];
                        }cout << endl;
                    }
                }
                moves++;
                cout <<endl<< "What is your move? [f/b][cm]" <<endl<<"For turning [r/l][1-8]"<<endl<<"To delete a move [d]"<<endl<< "To execute the route [e]"<<endl;
                cin >> tmove;
                if((tmove[0]=='f'||tmove[0]=='b'||tmove[0]=='r'||tmove[0]=='l')&&(tmove.size())<5){
                    msg.push_back({});
                    showMsg.push_back({});
                    switch (tmove[0]) {
                    case 'f':
                        move='c';
                        break;
                    case 'r':
                        move='b';
                        break;
                    case 'l':
                        move='a';
                        break;
                    case 'b':
                        move='d';
                        break;
                    default:
                        move ='c';
                        break;
                    }
                    msg[msgNr].push_back(move);
                    showMsg[msgNr].push_back(tmove[0]);
                    for (uint j=1;j<tmove.length();j++){
                        msg[msgNr].push_back(tmove[j]);
                        showMsg[msgNr].push_back(tmove[j]);
                        }msgNr++;
                    }
                else if(tmove == 'd'){
                    moves++;
                    msgNr--;
                    cout << endl;
                    for(uint m = 0;m<showMsg.size();m++){
                        cout << m+1 <<". ";
                        for(uint n =0;n<showMsg[m].size();n++){
                            cout << showMsg[m][n];
                        }cout << endl;
                    }
                    cout << endl << "Which entry do you wish to delete?"<<endl;
                    cin >> delIndex;
                    showMsg.erase(showMsg.begin()+delIndex-1);
                }
                else if(tmove == 'e'){
                    msg.push_back({'a','b','c'});
                    break;
                }
                else{cout << "\x1B[2J\x1B[H"<< "Invalid Input" << endl << " Please use either [f/r/l/b] and number in cm, or for a turn 1 to 8" <<endl;}
            }
            cout << "Complete Route: "<<endl << "{{";
            for (uint k=0;k<showMsg.size();k++){
                for(uint l=0;l<showMsg[k].size();l++){
                    if(l==showMsg[k].size()-1){
                        cout << showMsg[k][l];
                    }else{cout << showMsg[k][l]<<",";}
                }if(k!=showMsg.size()-1){cout << "},{";}
            }cout << "}}"<<endl<<endl;
            record(msg);

            //INSERT WAIT FOR ROBOT CONFIRMATION HERE

        }
        else if(tplay=='n'){
            play=tplay;
        }
        else{cout << "Invalid Input" << endl << " Please use either [y/n]" <<endl;}
    }
}

void Ui::record(vector<vector<char>> msg)
{
    sf::Sound sound;
    BufferMsg bM;
    MsgBuffer buf;
    MsgHandler handler;
    SeqHandler seq;
    sound.setVolume(100);

    for(int i = 0; i < (int)msg.size(); i++){

        cout << "Sending package Nr. " << i<<endl;
        cout << (i/(int)msg.size())*100<<"% Done"<<endl;

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

        vector<char> robReply = bM.recordSignal(5000);

        if(handler.isValid(robReply)){
            if(!seq.validatePN(robReply)){
                i--;
            }
        }else{
            i--;
        }
    }
}
