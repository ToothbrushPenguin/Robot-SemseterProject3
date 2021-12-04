#include <iostream>
#include <vector>
#include <complex>
#include <chrono>


using namespace std;
using namespace std::complex_literals;


vector<double> freqs = {697, 770, 852, 941, 1209, 1336, 1477, 1633};

vector<complex<double>> dft(vector<complex<double>> input)
{
    int N = input.size();
    vector<complex<double>> output = {};

    for(uint k = 0; k < freqs.size(); k++){
        double re = 0;
        double im = 0;
        for(int n = 0; n < N; n++)
        {
            re+= cos((2 * M_PI * freqs[k] * n) / N) * real(input[n]);
            im+= -sin((2 * M_PI * freqs[k] * n) / N) * real(input[n]);
        }
        complex<double> temp(re, im);
        output.push_back(temp);
    }
    return output;
}


vector<complex<double>> FastFourier(vector<complex<double>> msg)
{
    unsigned int n = msg.size();

    if(n == 1){
        return msg;
    }

    vector<complex<double>> pe(n/2,0);
    vector<complex<double>> po(n/2,0);

    for(unsigned int i = 0, j = 0; i <= n-2; i += 2, j++){
        pe[j]=msg[i];
        po[j]=msg[i+1];
        //pe.push_back(msg[i]);
        //po.push_back(msg[i+1]);
    }

    vector<complex<double>> ye = FastFourier(pe);
    vector<complex<double>> yo = FastFourier(po);

    complex<double> w(cos(-2*M_PI/n), sin(-2*M_PI/n));
    vector<complex<double>> y(n,0);

    for(unsigned int i = 0; i < n/2; i++){
        y[i] = ye[i]+(pow(w,i)*yo[i]);
        y[i+(n/2)] = ye[i]-(pow(w,i)*yo[i]);
    }

    return y;
}


int main()
{

    vector<int> samples = {};

    auto start = chrono::high_resolution_clock::now();

    double samplerate = 8192;
    vector<double> chanceoffrek = {};

    int oriL = samples.size();
    int npad = samplerate - oriL;
    int Fn = 1700;

    vector<complex<double>> input = {};

    for(int i = 0; i < oriL; i++){
        input.push_back((complex<double>)samples[i]);
    }

    for(int i = 0; i < npad; i++){
        input.push_back(0.);
    }

    vector<complex<double>> ftout = FastFourier(input);//skal endres til dft

    //------skal udkomenteres for dft------
    for(int i = 0; i < Fn-1; i++){
        chanceoffrek.push_back((abs(pow(ftout[i],2)))/ftout.size());
    }
    //-------------------------------------

    //------skal udkomenteres for fft------
    //vector<double> amps = {};
    //for(uint i = 0; i < ftout.size(); i++){
    //    amps.push_back(abs(ftout[i]));
    //}
    //-------------------------------------

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop-start);
    cout << "fft tager: " << duration.count() << "ms"<<endl;
}
