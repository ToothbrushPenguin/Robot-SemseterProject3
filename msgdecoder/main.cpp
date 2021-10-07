#include "msghandeler.h"

int main()
{

    MsgHandeler f;
    vector<char> eeee = {'c','c','1','2','3','4','6','0'};
    //cout << f.isValid(i) << endl;


    cout << f.DecodeMovement(f.incoder(eeee)) << endl;;


    return 0;
}
