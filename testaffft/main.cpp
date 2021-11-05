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
    vector<double> avchanceoffrek = {};
    int oriL = samples.size();
    int npad = samplerate - oriL;

    int Fn = 8000;

    vector<complex<double>> input = {};
    vector<complex<double>> fft = {};

    for(int i = 0; i < oriL; i++){
        input.push_back((complex<double>)samples[i]);
    }
    cout << input.size() << endl;
    cout << input.size() << endl;
    for(int i = 0; i < npad; i++){
        input.push_back(0.);
    }


    cout << "{\"";
    for(uint i = 0; i < input.size(); i++){
        if(i == input.size()-1){
            cout << real(input[i]);
        }else{
        cout <<real(input[i])<<", " ;}
    }
    cout << "\"}," << endl;

    cout << input.size() << endl;

    fft = FastFourier(input);


    for(int i = 0; i < Fn; i++){
        chanceoffrek.push_back(abs(pow(fft[i]/(complex<double>)oriL,2)));
    }


    //cout << "{\"";
    //for(uint i = 0; i < chanceoffrek.size(); i++){
    //    if(i == chanceoffrek.size()-1){
    //        cout << chanceoffrek[i];
    //    }else{
    //    cout <<chanceoffrek[i]<<", " ;}
    //}
    //cout << "\"}," << endl;


    return 1;
}


int main()
{
    vector<double> sampels = {};


    //for (double j = 0; j < 1; j+=(1/(samplerate))){
    //    sampels.push_back((100*sin(M_PI*697*j)));
    //}
    //cout << sampels.size() << endl;
    //cout << "{\"";
    //for(uint i = 0; i < sampels.size(); i++){
    //    if(i == sampels.size()-1){
    //        cout << sampels[i];
    //    }else{
    //    cout <<sampels[i]<<", " ;}
    //}
    //cout << "\"}," << endl;

    for (double j = 0; j < 0.05; j+=(1/(samplerate))){
        sampels.push_back((10000*sin(M_PI*697*j)));
    }



    FourierSplit(sampels);


    return 0;
}
