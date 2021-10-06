#include "msghandeler.h"

int main()
{

    MsgHandeler f;
    vector<char> i = {'a','a','1','2','3','4','d','c','4','5',  '0'};
    //cout << f.isValid(i) << endl;
    f.incoder(i);
    //f.isValid(i);
    f.decodeValue(f.incoder(i));
    return 0;
}
