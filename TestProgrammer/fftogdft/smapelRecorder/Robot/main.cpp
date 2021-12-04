#include <iostream>
#include "../Classes/buffermsg.h"
#include "../Classes/msgbuffer.h"
#include "../Classes/msghandler.h"
#include "../Classes/robotmovement.h"

using namespace std;



int main()
{
    BufferMsg bM;
    vector<char> message = bM.recordSignal();
}
