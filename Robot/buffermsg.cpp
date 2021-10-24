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
                    samplefile.open ("samplefile.txt");
                    samplefile << out;
                    samplefile.close();
                    cout << "Should work" << endl;
                }
                if(result(fsout) == '*'){
                    toggle = 1;
                }               
            }

        }
        return {0};
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
    auto middle = next(chancein.begin(),chancein.size()/2);
    vector<int> first4low(chancein.begin(),middle);
    vector<int> larst4high(middle,chancein.end());
    double first4max =*max_element(first4low.begin(),first4low.end());
    double larst4max =*max_element(larst4high.begin(),larst4high.end());
    double biggest2sum = first4max+larst4max;
    double firstsum = accumulate(first4low.begin(), first4low.end(), 0);
    double lastsum = accumulate(larst4high.begin(), larst4high.end(), 0);
    double allsum = accumulate(chancein.begin(), chancein.end(), 0);


    int lowfrek=0;
    int highfrek=0;

    if(biggest2sum/allsum > 0.69 && first4max/firstsum > 0.6 && larst4max/lastsum > 0.6){

        for(int s = 0; s<8; s++){
            cout << chancein[s] << "\n";
        }
        cout << endl;

        for(unsigned int u = 0; u < first4low.size(); u++){
            if(first4low[u]==first4max){
                lowfrek=(int)frequencys[u];
            }
            if(larst4high[u]==larst4max){
                highfrek=(int)frequencys[u+4];
            }
        }

    }else{
        return vector<int>(1,-1);
    }

    return {lowfrek,highfrek};
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




