#include "buffermsg.h"

using namespace std;

int main()
{
    BufferMsg a;

    vector<char> e = a.SignalRecord();

    for(unsigned int u = 0; u < e.size(); u++){
        cout << e[u] << endl;
    }

    return 0;
}
