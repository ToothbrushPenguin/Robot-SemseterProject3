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

            fsout = FourierSplit(rec);
            cout << result(fsout) << endl;

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

                    //ofstream samplefile;
                    //samplefile.open ("samplefile.txt");
                    //samplefile << out;
                    //samplefile.close();
                    //cout << "Should work" << endl;
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
    samples = {32767,21071,-25387,-32768,-18824,32767,7210,-32768,-32768,-28911,20949,32767,32767,13794,-31191,-32768,-23090,31727,14138,-32768,-32768,-20837,27773,32767,32767,6259,-32768,-32768,-25919,29574,21572,-29920,-32768,-15324,29802,32767,32767,-1334,-32768,-32768,-27839,25373,32767,-18448,-32768,-8993,31934,32767,32767,-10215,-32768,-32768,-24889,25758,32767,-7115,-32768,-2334,32767,32767,31991,-16137,-32768,-32768,-21213,26804,32767,3455,-32768,3356,32767,32767,25956,-25086,-32768,-32768,-13959,30586,32767,11973,-32768,642,32767,32767,19409,-29954,-32768,-32768,-4031,32767,32767,17113,-32768,-8785,32767,32767,11833,-32418,-32768,-32768,2951,32767,32767,20355,-32768,-19778,30654,32767,3630,-32768,-32768,-32768,12231,32767,32767,20116,-32703,-31843,19652,32767,-1122,-32768,-32768,-30629,17946,32767,32767,16770,-32136,-32768,10749,32767,-7290,-32768,-32768,-24616,26067,32767,32767,10385,-32768,-32768,-854,32767,-8320,-32768,-32768,-17089,31461,32767,32767,387,-32768,-32768,-10579,32767,-4612,-32768,-32768,-10031,32767,32767,32767,-5897,-32768,-32768,-16640,32767,6625,-32768,-32768,-10,32767,32767,32253,-15625,-32768,-32768,-17321,32767,18478,-32415,-32768,6900,32767,32767,27569,-22665,-32768,-32768,-14550,32767,26206,-26788,-32768,13290,32767,32767,20145,-29558,-32768,-32768,-8409,32767,32767,-15299,-32768,15767,32767,32767,14495,-32768,-32768,-32768,982,32767,32767,-3533,-32768,14840,32767,32767,6044,-32768,-32709,-32768,7667,32767,32767,5003,-32768,8420,32767,32767,-2148,-32768,-32768,-32476,15090,32767,32767,10165,-32768,-3197,32767,32767,-11139,-32768,-32768,-26436,24152,32767,32767,8701,-32768,-15985,32767,32767,-15925,-32768,-32768,-20390,29053,32767,32767,2119,-32768,-25623,27101,32767,-20687,-32768,-32768,-11608,32767,32767,32767,-4950,-32768,-32768,13229,32767,-21301,-32768,-32768,-2887,32767,32767,32733,-12105,-32768,-32768,4869,32767,-17678,-32768,-32768,5946,32767,32767,29822,-19647,-32768,-32768,-1028,32767,-10959,-32768,-31199,13227,32767,32767,23003,-27372,-32768,-32768,-648,32767,1290,-32768,-28250,20619,32767,32767,17826,-30909,-32768,-32768,1684,32767,14305,-32768,-27045,25285,32767,32767,8859,-32768,-32768,-32768,5455,32767,24865,-29132,-24658,29532,32767,32767,-1185,-32768,-32768,-30947,14423,32767,31546,-18158,-30936,24516,32767,32767,-7568,-32768,-32768,-28161,20711,32767,32767,-10207,-31437,23287,32767,30211,-15308,-32768,-32768,-21669,28569,32767,32767,-4137,-32768,14253,32767,26443,-24906,-26672,30295,32767,-10253,-32768,-30931};
    vector<double> chanceoffrek = {};
    vector<double> avchanceoffrek = {};
    int oriL = samples.size();
    int npad = samplerate - oriL;
    int padSide = 10;
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


    //for(uint i = 0; i < chanceoffrek.size(); i++){
    //    cout <<", " <<chanceoffrek[i];
    //}
    //cout  << endl<< endl;

    for(int i = 0; i < Fn-1; i++){
        chanceoffrek.push_back(abs(pow(fft[i],2))/(double)fft.size());
    }

    //for(uint i = 0; i < chanceoffrek.size(); i++){
    //    cout <<", " <<chanceoffrek[i];
    //}
    //cout  << endl<< endl;



    for(unsigned int i = 0; i <= chanceoffrek.size(); i++){
        if(i%125==0){
            if(i!=0 || i!=chanceoffrek.size()){
                chanceoffrek.at(i) = 0;
            }else{
                chanceoffrek.at(i) = (chanceoffrek.at(i-1) + chanceoffrek.at(i+1)) / 2;
            }
        }
    }

    for(int i = 0; i < padSide; i++){
        avchanceoffrek.push_back(chanceoffrek.at(i));

    }
    for(unsigned int i = padSide; i < chanceoffrek.size()-padSide; i++){
        double sum = 0;
        int devider = 0;
        for(int j = -padSide; j <= padSide; j++){
            sum += chanceoffrek.at(i+j);
            devider++;
        }
        avchanceoffrek.push_back(sum/devider);
    }
    for(unsigned int i = chanceoffrek.size()-padSide; i < chanceoffrek.size(); i++){
        avchanceoffrek.push_back(chanceoffrek.at(i));
    }


    //for(uint i = 0; i < avchanceoffrek.size(); i++){
    //    cout <<", " <<avchanceoffrek[i];
    //}
    //cout  << endl<< endl;


    return twoLargest(avchanceoffrek);
}

vector<int> BufferMsg::twoLargest(vector<double> chancein)
{
    //værdien for de forskellige peaks
    vector<double> peaks = {};
    vector<double> peaksIDX = {};

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

    if((((largestPeaks.at(0)+largestPeaks.at(1))/largestPeaks.at(2)) > 2 && ((largestEight.at(0)+largestEight.at(1))/largestEight.at(2)) > 2) && (((largesteightIDX.at(0) < 1000) && (largesteightIDX.at(1) > 1000))||((largesteightIDX.at(0) > 1000) && (largesteightIDX.at(1) < 1000)))){
        if(largesteightIDX.at(1) < largesteightIDX.at(0)){
            return {largesteightIDX.at(1),largesteightIDX.at(0)};
        }
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

vector<complex<double>> BufferMsg::FastFourier(vector<complex<double>> fsbuf)
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



