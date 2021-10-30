#include "buffermsg.h"

BufferMsg::BufferMsg()
{
    if (SignalRecoder::isAvailable())
    {
    recorder.start(samplerate);
    this_thread::sleep_for(chrono::milliseconds(2000));
    }
}

vector<char> BufferMsg::SignalRecord(int timeout)
{    
    int sleeptime = 25;
    bool toggle = 0;
    int time = 0;
    vector<char> msg = {};
        while(timeout == -1 || time < timeout){

            time += sleeptime;
            this_thread::sleep_for(chrono::milliseconds(sleeptime));


            vector<int> rec = recorder.getSamp();

            fsout = FourierSplit(recorder.getSamp());



            if(fsout.size()==2){
                if(result(fsout) == '#'){//stop bit
                    return msg;
                }
                if(toggle == 1&&result(fsout) != '*'){
                    msg.push_back(result(fsout));

                    string out = "";
                    for (unsigned long i = 0; i < rec.size(); i++){
                        out += to_string(rec[i]);
                        if(i != rec.size()-1){
                            out += ",";
                        }
                    }

                    ofstream samplefile;
                    samplefile.open ("samplefile1.txt");
                    samplefile << out;
                    samplefile.close();
                    cout << "Should work" << endl;
                }
                if(result(fsout) == '*'){
                    toggle = 1;
                }               
            }

        }
        return {'0'};
}

BufferMsg::~BufferMsg()
{
    recorder.stop();
}


vector<int> BufferMsg::FourierSplit(vector<int> samples)
{
    vector<double> chanceoffrek = {};
    vector<int> largest = {};
    complex<double> number = 0;
    vector<complex<double>> comp = {};

    for(unsigned int u = 0; u < frequencys.size(); u++){
        number = 0;
        for(unsigned int n = 0; n < samples.size()-1; n++){
            number += (double)samples[n]*exp((-2i*frequencys[u]*(double)n*M_PI)/(double)samplerate);
        }
        comp.push_back(number*2./(double)samples.size());
    }

    for(unsigned int u = 0; u < frequencys.size(); u++){
        chanceoffrek.push_back((sqrt(pow(real(comp[u]),2)+pow(imag(comp[u]),2))));
    }

    return twoLargest(chanceoffrek);
}

vector<int> BufferMsg::twoLargest(vector<double> chancein)
{
    //værdien for de forskellige peaks
    vector<double> peaks;
    vector<double> peaksIDX;
    for(unsigned int i = 1; i<=chancein.size()-2;i++){
        if(chancein[i-1]<chancein[i] && chancein[i+1]<chancein[i]){
            peaks.push_back(chancein[i]);
            peaksIDX.push_back(i);
        }
    }
    //de 8 frekvnesers værdi
    vector<double> eight={};
    vector<double> eightIDX={};
    for(unsigned int i = 0; i<=chancein.size();i++){
        if(i==frequencys[0]||i==frequencys[1]||i==frequencys[2]||i==frequencys[3]||i==frequencys[4]||i==frequencys[5]||i==frequencys[6]||i==frequencys[7]){
            eight.push_back(chancein[i]);
            eightIDX.push_back(i);
        }
    }

    //finding the 3 largest of the 8 dtmf freqencies
    vector<int> largesteightIDX={0,0,0};
    vector<double> largestEight={0,0,0};
    for(unsigned int i = 0; i<=eight.size();i++){
        if(eight[i]>largestEight[0]){
            largestEight.at(1)=largestEight.at(0);
            largesteightIDX.at(1)=largesteightIDX.at(0);
            largestEight.at(0)=eight[i];
            largesteightIDX.at(0)=eightIDX[i];
        }else if(eight[i]>largestEight[1]){
            largestEight.at(2)=largestEight.at(1);
            largesteightIDX.at(2)=largesteightIDX.at(1);
            largestEight.at(1)=eight[i];
            largesteightIDX.at(1)=eightIDX[i];
        }else if(eight[i]>largestEight[2]){
            largestEight.at(2)=eight[i];
            largesteightIDX.at(2)=eightIDX[i];
        }
    }

    //finding the 3 largest peaks
    vector<int> largestPeaksIDX={0,0,0};
    vector<double> largestPeaks={0,0,0};
    for(unsigned int i = 0; i<=peaks.size();i++){
        if(peaks[i]>largestPeaks[0]){
            largestPeaks.at(1)=largestPeaks.at(0);
            largestPeaksIDX.at(1)=largestPeaksIDX.at(0);
            largestPeaks.at(0)=peaks[i];
            largestPeaksIDX.at(0)=peaksIDX[i];
        }else if(peaks[i]>largestPeaks[1]){
            largestPeaks.at(2)=largestPeaks.at(1);
            largestPeaksIDX.at(2)=largestPeaksIDX.at(1);
            largestPeaks.at(1)=peaks[i];
            largestPeaksIDX.at(1)=peaksIDX[i];
        }else if(peaks[i]>largestPeaks[2]){
            largestPeaks.at(2)=peaks[i];
            largestPeaksIDX.at(2)=peaksIDX[i];
        }
    }

    if(((largestPeaks.at(0)+largestPeaks.at(1))/largestPeaks.at(2)) > 5 && ((largestEight.at(0)+largestEight.at(1))/largestEight.at(2)) > 5){
        return {largesteightIDX.at(0),largesteightIDX.at(1)};
    }
    return{0,0};
}

char BufferMsg::result(vector<int> frequency)
{

    vector<int> n;
    if(frequency[0]!=0&&frequency[1]!=0){
        switch(frequency[0])
        {
        case 697:n={'1','2','3','a'} ; break;
        case 770:n={'4','5','6','b'} ; break;
        case 852:n={'7','8','9','c'} ; break;
        case 941:n={'*','0','#','d'} ; break;
        }

        int m;
        switch(frequency[1])
        {
        case 1209:m=0 ; break;
        case 1336:m=1 ; break;
        case 1477:m=2 ; break;
        case 1633:m=3 ; break;
        }
        return n[m];
    }
    return 'N';
}

vector<complex<double>> FastFourier(vector<complex<double>> fsbuf)
{
    //cout << "hello"<<endl;
    int n = fsbuf.size();
    vector<complex<double>> pe = {};
    vector<complex<double>> po = {};
    vector<complex<double>> ye;
    vector<complex<double>> yo;
    if (n==1){
        return fsbuf;
    }
    complex<double> w = exp((-2*M_PI*1i)/(double) n);
    for (uint i = 0; i<fsbuf.size();i++){
        if(i % 2 == 0){
            pe.push_back(fsbuf.at(i));
        }else{
            po.push_back(fsbuf.at(i));
        }
    }

    ye = FastFourier(pe);
    yo = FastFourier(po);
    //cout << "goodbye"<<endl;
    vector<complex<double>> y;
    vector<complex<double>> y1;
    vector<complex<double>> y2;

    for (int j = 0;j<n/2;j++){
        y1.push_back(ye[j]+pow(w,j)*yo[j]);
        y2.push_back(ye[j]-pow(w,j)*yo[j]);
    }
    for (uint k = 0;k<y1.size()+y2.size();k++){
        if(k<y1.size()){
            y.push_back(y1.at(k));
        }else{
            y.push_back(y2.at(k-y1.size()));
        }
    }
    return y;
}




