#ifndef UI_H
#define UI_H
#include "msghandler.h"
#include "seqhandler.h"
#include "buffermsg.h"

using namespace std;

class Ui
{
public:
    Ui();
    void runUi();
    void record(vector<vector<char>> msg);
};

#endif // UI_H
