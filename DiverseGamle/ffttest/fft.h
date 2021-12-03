#ifndef FFT_H
#define FFT_H
#include <vector>
#include <complex>
#include <math.h>

using namespace std;

class fft
{
public:
    fft();
    vector<complex<double>> ifft(vector<complex<double>> msg);
};

#endif // FFT_H
