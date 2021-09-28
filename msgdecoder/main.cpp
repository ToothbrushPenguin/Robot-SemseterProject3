#include "msghandeler.h"

int main()
{
    cout << "Hello World!" << endl;
    MsgHandeler f;
    vector<char> i = {'A','A','1','D','C','5','A','D','1','3','6'};
    cout << f.isValid(i) << endl;
    return 0;
}
