#include <iostream>
#include <complex>
#include <vector>
#include <math.h>
#include <chrono>


using namespace std;
using namespace std::complex_literals;
double samplerate = 8000;

vector<complex<double>> FastFourier(vector<complex<double>> fsbuf)
{
    //cout << "hello"<<endl;
    int n = fsbuf.size();
    vector<complex<double>> pe = {};
    vector<complex<double>> po = {};
    vector<complex<double>> ye = {};
    vector<complex<double>> yo = {};
    if (n==1){
        return fsbuf;
    }
    complex<double> w = exp((-2*M_PI*1i)/(double)n);
    for (uint i = 0; i < fsbuf.size(); i++){
        if(i % 2 == 0){
            pe.push_back(fsbuf.at(i));
        }else{
            po.push_back(fsbuf.at(i));
        }
    }

    ye = FastFourier(pe);
    yo = FastFourier(po);
    //cout << "goodbye"<<endl;
    vector<complex<double>> y = {};
    vector<complex<double>> y1 = {};
    vector<complex<double>> y2 = {};

    for (int j = 0; j < n/2; j++){
        y1.push_back(ye[j]+pow(w,j)*yo[j]);
        y2.push_back(ye[j]-pow(w,j)*yo[j]);
    }
    for (uint k = 0; k < y1.size() + y2.size(); k++){
        if(k < y1.size()){
            y.push_back(y1.at(k));
        }else{
            y.push_back(y2.at(k-y1.size()));
        }
    }
    return y;
}

int FourierSplit(vector<double> samples)
{
    vector<double> chanceoffrek = {};
    int oriL = samples.size();
    int npad = samplerate - oriL;
    int Fn = 1700;

    vector<complex<double>> input = {};
    vector<complex<double>> fft = {};

    for(int i = 0; i < oriL; i++){
        input.push_back((complex<double>)samples[i]);
    }

    for(int i = 0; i < npad; i++){
        input.push_back(0.);
    }

    fft = FastFourier(input);

    for(int i = 0; i < Fn; i++){
        chanceoffrek.push_back(abs(pow(fft[i],2))/(double)fft.size());
    }

    cout << chanceoffrek.size()<< endl;

    cout << "{\"";
    for(uint i = 0; i < chanceoffrek.size(); i++){
        if(i == chanceoffrek.size()-1){
            cout << chanceoffrek[i];
        }else{
        cout <<chanceoffrek[i]<<", " ;}
    }
    cout << "\"}," << endl;


    return 1;
}


int main()
{
    vector<double> sampels = {};

    for (double j = 0; j < 1; j+=0.001){
        sampels.push_back(10000*sin(M_PI*1000*j));
    }
    FourierSplit(sampels);


    return 0;
}
