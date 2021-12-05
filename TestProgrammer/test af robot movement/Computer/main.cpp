#include "../Classes/ui.h"

using namespace std;

int main()
{
    Ui u;

    vector<vector<char>> msg1 ={{'c','2','5'},{'d','2','5'},{'a','b','c'}};
    u.record(msg1);

    u.runUi();
    return 0;
}
