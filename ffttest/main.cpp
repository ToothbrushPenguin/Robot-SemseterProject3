#include <iostream>
#include "fft.h"
using namespace std;

int main()
{
    fft test;
    vector<complex<double>> msg = {};
    vector<complex<double>>out = test.ifft(msg);
    for(unsigned int i = 0; i < out.size(); i++){
        cout << abs(out[i])/out.size() << ",";
    }
    cout << endl;
}
