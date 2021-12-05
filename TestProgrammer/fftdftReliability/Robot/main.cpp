#include <iostream>
#include "../Classes/buffermsg.h"
#include "../Classes/msgbuffer.h"
#include "../Classes/msghandler.h"
#include "../Classes/robotmovement.h"

using namespace std;

int main()
{
    BufferMsg bM;

    vector<char> succes ={'a','1','1','5','9','6','1','4'};

    double succ = 0;
    double fail = 0;
    double succFFT = 0;
    double failFFT = 0;

    for(int i = 0; i < 20; i++){

        vector<char> message = bM.recordSignal();


        if(succes == message){
            succ++;
        }else{
            fail++;
        }

        cout << "messageDFT      :   ";
        for(uint i = 0; i < message.size(); i++){
            cout << message[i];
        }
        cout << endl;
        cout <<  "success&fail DFT:   " << succ << " : " << fail << endl;
        cout <<  "success rate DFT:   " <<(succ/(succ+fail))*100  << "%" << endl;

        vector<char> messageFFT = bM.recordSignalFFT();


        if(succes == messageFFT){
            succFFT++;
        }else{
            failFFT++;
        }

        cout << "messageFFT      :   ";
        for(uint i = 0; i < messageFFT.size(); i++){
            cout << messageFFT[i];
        }
        cout << endl;
        cout <<  "success&fail FFT:   " << succFFT << " : " << failFFT << endl;
        cout <<  "success rate FFT:   " <<(succFFT/(succFFT+failFFT))*100  << "%" << endl;


    }



}
