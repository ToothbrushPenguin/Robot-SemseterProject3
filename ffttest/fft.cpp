#include "fft.h"

fft::fft()
{
    
}

vector<complex<double>> fft::ifft(vector<complex<double>> msg)
{
    unsigned int n = msg.size();
    
    if(n == 1){
        return msg;
    }
    
    vector<complex<double>> pe = {};
    vector<complex<double>> po = {};
    
    for(unsigned int i = 0; i <= n-2; i += 2){
        pe.push_back(msg.at(i));
        po.push_back(msg.at(i+1));
    }
    
    vector<complex<double>> ye = ifft(pe);
    vector<complex<double>> yo = ifft(po);
    
    complex<double> w(cos(-2*M_PI/n), sin(-2*M_PI/n));
    vector<complex<double>> y(n,0);
    
    for(unsigned int i = 0; i < n/2; i++){
        y.at(i) = ye.at(i)+(pow(w,i)*yo.at(i));
        y.at(i+(n/2)) = ye.at(i)-(pow(w,i)*yo.at(i));
    }
    
    return y;
}
