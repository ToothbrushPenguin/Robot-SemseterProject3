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

    for(int i = 0; i < 20; i++){




        vector<char> message = bM.recordSignal();


        if(succes == message){
            succ++;
        }else{
            fail++;
        }

        cout << "message      :   ";
        for(uint i = 0; i < message.size(); i++){
            cout << message[i];
        }
        cout << endl;
        cout <<  "success&fail :   " << succ << " : " << fail << endl;
        cout <<  "success rate :   " <<(succ/(succ+fail))*100  << "%" << endl;


    }



}
