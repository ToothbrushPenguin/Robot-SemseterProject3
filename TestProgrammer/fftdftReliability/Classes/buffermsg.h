#ifndef BUFFERMSG_H
#define BUFFERMSG_H

#include "signalrecorder.h"

#include <fstream>
#include <numeric>

class BufferMsg
{
public:
    BufferMsg();
    vector<char> recordSignal(int timeout = -1);
    vector<char> recordSignalFFT(int timeout = -1);
    vector<vector<int>> temprecFFT = {};
    ~BufferMsg();

private:
    vector<int> fourierSplit(vector<int> samples);//retunere de 2 frekvencer
    vector<int> fourierSplitFFT(vector<int> samples);
    vector<complex<double>> fastFourier(vector<complex<double>> msg);
    vector<int> twoLargest(vector<double> chancein);
    char result(vector<int> frequency);
    vector<complex<double>> dft(vector<complex<double>> input);
    int largestInList(vector<double>list);
    vector<int> winFunc(vector<int> samp);
    vector<char> interpreter(vector<char>list);

    vector<double> freqs = {697, 770, 852, 941, 1209, 1336, 1477, 1633};
    double samplerate = 8192;
    SignalRecorder recorder;
    int oldrecLength = 0;
    vector<int> rec={};




    //skal fjernes
    ofstream samplefile;
};

#endif // BUFFERMSG_H
