#include "buffermsg.h"

BufferMsg::BufferMsg()
{
    //test--
    samplefile.open ("samplefile.txt");
    //------
    if (SignalRecoder::isAvailable())
    {
    recorder.start(samplerate);
    this_thread::sleep_for(chrono::milliseconds(2000));
    }

}

vector<char> BufferMsg::SignalRecord(int timeout)
{    
    int sleeptime = 50;
    bool toggle = 0;
    int time = 0;
    vector<char> msg = {};
        while(timeout == -1 || time < timeout){

            time += sleeptime;
            this_thread::sleep_for(chrono::milliseconds(sleeptime));


            vector<int> temprec = recorder.getSamp();

            for(unsigned int i = 0; i < temprec.size(); i++){
                rec.push_back(temprec.at(i));
            }

            fsout = FourierSplit(rec);

            for(int i = 0; i < oldrecLength; i++){
                rec.erase(rec.begin());
            }
            oldrecLength = rec.size();

            cout << "{\""<<result(fsout)<<"\"}," << endl;

            if(fsout.size()==2){
                if(result(fsout) == '#'){//stop bit
                    vector<char> d = dumb(msg);
                    for(uint i = 0; i < d.size(); i++){
                        cout << d.at(i);
                    }
                    return dumb(msg);
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
                    cout << "Samplefile printed" << endl;
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
    //test---
    samplefile.close();
    //----
    recorder.stop();
}


vector<int> BufferMsg::FourierSplit(vector<int> samples)
{


    //for(uint i = 0; i < samples.size(); i++){
    //    if(i == samples.size()-1){
    //        samplefile << samples[i];
    //    }else{
    //    samplefile <<samples[i]<<", " ;}
    //}



    samples = {32767,21071,-25387,-32768,-18824,32767,7210,-32768,-32768,-28911,20949,32767,32767,13794,-31191,-32768,-23090,31727,14138,-32768,-32768,-20837,27773,32767,32767,6259,-32768,-32768,-25919,29574,21572,-29920,-32768,-15324,29802,32767,32767,-1334,-32768,-32768,-27839,25373,32767,-18448,-32768,-8993,31934,32767,32767,-10215,-32768,-32768,-24889,25758,32767,-7115,-32768,-2334,32767,32767,31991,-16137,-32768,-32768,-21213,26804,32767,3455,-32768,3356,32767,32767,25956,-25086,-32768,-32768,-13959,30586,32767,11973,-32768,642,32767,32767,19409,-29954,-32768,-32768,-4031,32767,32767,17113,-32768,-8785,32767,32767,11833,-32418,-32768,-32768,2951,32767,32767,20355,-32768,-19778,30654,32767,3630,-32768,-32768,-32768,12231,32767,32767,20116,-32703,-31843,19652,32767,-1122,-32768,-32768,-30629,17946,32767,32767,16770,-32136,-32768,10749,32767,-7290,-32768,-32768,-24616,26067,32767,32767,10385,-32768,-32768,-854,32767,-8320,-32768,-32768,-17089,31461,32767,32767,387,-32768,-32768,-10579,32767,-4612,-32768,-32768,-10031,32767,32767,32767,-5897,-32768,-32768,-16640,32767,6625,-32768,-32768,-10,32767,32767,32253,-15625,-32768,-32768,-17321,32767,18478,-32415,-32768,6900,32767,32767,27569,-22665,-32768,-32768,-14550,32767,26206,-26788,-32768,13290,32767,32767,20145,-29558,-32768,-32768,-8409,32767,32767,-15299,-32768,15767,32767,32767,14495,-32768,-32768,-32768,982,32767,32767,-3533,-32768,14840,32767,32767,6044,-32768,-32709,-32768,7667,32767,32767,5003,-32768,8420,32767,32767,-2148,-32768,-32768,-32476,15090,32767,32767,10165,-32768,-3197,32767,32767,-11139,-32768,-32768,-26436,24152,32767,32767,8701,-32768,-15985,32767,32767,-15925,-32768,-32768,-20390,29053,32767,32767,2119,-32768,-25623,27101,32767,-20687,-32768,-32768,-11608,32767,32767,32767,-4950,-32768,-32768,13229,32767,-21301,-32768,-32768,-2887,32767,32767,32733,-12105,-32768,-32768,4869,32767,-17678,-32768,-32768,5946,32767,32767,29822,-19647,-32768,-32768,-1028,32767,-10959,-32768,-31199,13227,32767,32767,23003,-27372,-32768,-32768,-648,32767,1290,-32768,-28250,20619,32767,32767,17826,-30909,-32768,-32768,1684,32767,14305,-32768,-27045,25285,32767,32767,8859,-32768,-32768,-32768,5455,32767,24865,-29132,-24658,29532,32767,32767,-1185,-32768,-32768,-30947,14423,32767,31546,-18158,-30936,24516,32767,32767,-7568,-32768,-32768,-28161,20711,32767,32767,-10207,-31437,23287,32767,30211,-15308,-32768,-32768,-21669,28569,32767,32767,-4137,-32768,14253,32767,26443,-24906,-26672,30295,32767,-10253,-32768,-30931};
    vector<double> chanceoffrek = {};
    vector<double> avchanceoffrek = {};
    int oriL = samples.size();
    int npad = samplerate - oriL;
    int padSide = 10;
    int Fn = 1700;

    vector<complex<double>> input = {};
    vector<complex<double>> fft = {};

    vector<int> nsamples;
    nsamples=triWinFunk(samples);

    for(int i = 0; i < oriL; i++){
        input.push_back((complex<double>)nsamples[i]);
    }

    for(int i = 0; i < npad; i++){
        input.push_back(0.);
    }



    auto start = chrono::high_resolution_clock::now();

    fft = FastFourier(input);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    cout << "Time: " << duration.count() << endl;



    for(int i = 0; i < Fn-1; i++){
        chanceoffrek.push_back((abs(pow(fft[i],2)))/fft.size());//chanceoffrek.push_back((abs(fft[i])*2)/fft.size());
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


    //print----
//    samplefile << "{\"";
//    for(uint i = 0; i < avchanceoffrek.size(); i++){
//        if(i == avchanceoffrek.size()-1){
//            samplefile << avchanceoffrek[i];
//        }else{
//        samplefile <<avchanceoffrek[i]<<", " ;}
//    }
//    samplefile << "\"}," << endl;
    //--------



    return twoLargest(avchanceoffrek);
}

vector<int> BufferMsg::twoLargest(vector<double> chancein)
{
    double lowerBound = 500000;
    double lowerRatio = 3;

    //de 8 frekvnesers værdi
    vector<double> eightL={};
    vector<int> eightLFrek={};
    double avgL=0;
    vector<double> eightH={};
    vector<int> eightHFrek={};
    double avgH=0;

    for(unsigned int i = 0; i<=chancein.size();i++){
        switch (i) {
        case 697:
            eightL.push_back(chancein[i]);
            eightLFrek.push_back(697);
            avgL+=chancein[i];
            break;
        case 770:
            eightL.push_back(chancein[i]);
            eightLFrek.push_back(770);
            avgL+=chancein[i];
            break;
        case 852:
            eightL.push_back(chancein[i]);
            eightLFrek.push_back(852);
            avgL+=chancein[i];
            break;
        case 941:
            eightL.push_back(chancein[i]);
            eightLFrek.push_back(941);
            avgL+=chancein[i];
            break;
        case 1209:
            eightH.push_back(chancein[i]);
            eightHFrek.push_back(1209);
            avgH+=chancein[i];
            break;
        case 1336:
            eightH.push_back(chancein[i]);
            eightHFrek.push_back(1336);
            avgH+=chancein[i];
            break;
        case 1477:
            eightH.push_back(chancein[i]);
            eightHFrek.push_back(1477);
            avgH+=chancein[i];
            break;
        case 1633:
            eightH.push_back(chancein[i]);
            eightHFrek.push_back(1633);
            avgH+=chancein[i];
            break;
        default:
            break;
        }

    }

    avgL=avgL/4;
    avgH=avgH/4;

    int largestIDXL = LargestInList(eightL);
    int largestIDXH = LargestInList(eightH);

    //cout << " H: " <<avgH << endl <<" L: " << avgL << endl;
    //cout << " H: " <<largestIDXH << endl <<" L: " << largestIDXL << endl;
    //cout << " H: " <<eightHFrek[largestIDXH] <<" L: " << eightLFrek[largestIDXL] << endl;

    if((eightL[largestIDXL]>avgL*lowerRatio && eightL[largestIDXL]>lowerBound) && (eightH[largestIDXH]>avgH*lowerRatio && eightH[largestIDXH]>lowerBound)){

        return{eightLFrek[largestIDXL],eightHFrek[largestIDXH]};
    }
    return{0,0};


    ////værdien for de forskellige peaks
    //vector<double> peaks = {};
    //vector<double> peaksIDX = {};
    //
    //for(unsigned int i = 1; i<=chancein.size()-2;i++){
    //    if(chancein[i-1]<chancein[i] && chancein[i+1]<chancein[i]){
    //        peaks.push_back(chancein[i]);
    //        peaksIDX.push_back(i);
    //    }
    //}
    //
    ////finding the 3 largest peaks
    //vector<int> largestPeaksIDX={0,0,0};
    //vector<double> largestPeaks={0,0,0};
    //for(unsigned int i = 0; i<=peaks.size();i++){
    //    if(peaks[i]>largestPeaks[0]){
    //        largestPeaks.at(1)=largestPeaks.at(0);
    //        largestPeaksIDX.at(1)=largestPeaksIDX.at(0);
    //        largestPeaks.at(0)=peaks[i];
    //        largestPeaksIDX.at(0)=peaksIDX[i];
    //    }else if(peaks[i]>largestPeaks[1]){
    //        largestPeaks.at(2)=largestPeaks.at(1);
    //        largestPeaksIDX.at(2)=largestPeaksIDX.at(1);
    //        largestPeaks.at(1)=peaks[i];
    //        largestPeaksIDX.at(1)=peaksIDX[i];
    //    }else if(peaks[i]>largestPeaks[2]){
    //        largestPeaks.at(2)=peaks[i];
    //        largestPeaksIDX.at(2)=peaksIDX[i];
    //    }
    //}
    //
    //cout << largestPeaks.at(0)<<" : "<<largestPeaks.at(1)<<" : "<<largestPeaks.at(2) << endl;
    // cout << largestEight.at(0)<<" : "<<largestEight.at(1) << endl;
    //if((((largestPeaks.at(0)+largestPeaks.at(1))/largestPeaks.at(2)) > (M_PI-1) && ((largestEight.at(0)+largestEight.at(1))/largestPeaks.at(2)) > (M_PI-1)) && (((largesteightIDX.at(0) < 1000) && (largesteightIDX.at(1) > 1000))||((largesteightIDX.at(0) > 1000) && (largesteightIDX.at(1) < 1000)))){
    //    if(largesteightIDX.at(1) < largesteightIDX.at(0)){
    //        return {largesteightIDX.at(1),largesteightIDX.at(0)};
    //    }
    //    return {largesteightIDX.at(0),largesteightIDX.at(1)};
    //}
    //return{0,0};
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

vector<complex<double>> BufferMsg::FastFourier(vector<complex<double>> msg)
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

    vector<complex<double>> ye = FastFourier(pe);
    vector<complex<double>> yo = FastFourier(po);

    complex<double> w(cos(-2*M_PI/n), sin(-2*M_PI/n));
    vector<complex<double>> y(n,0);

    for(unsigned int i = 0; i < n/2; i++){
        y.at(i) = ye.at(i)+(pow(w,i)*yo.at(i));
        y.at(i+(n/2)) = ye.at(i)-(pow(w,i)*yo.at(i));
    }

    return y;
}

int BufferMsg::LargestInList(vector<double> list)
{
    //finding the largest value of a lsit and return the index of it
    double largest=0;
    int largestIDX=0;
    for(unsigned int i = 0; i<list.size();i++){
        if(list[i]>largest){
            largest=list[i];
            largestIDX=i;
        }
    }
    return largestIDX;
}

vector<int> BufferMsg::triWinFunk(vector<int> samp)
{
    vector<int> nsamp={};
    int n=samp.size();
    //for(int i = 0; i < (int)samp.size();i++){
    //    if(i<n/2){
    //        nsamp.push_back(samp.at(i)*(2*(i/(n-1))));
    //    }else{
    //        nsamp.push_back(samp.at(i)*(2-(2*(i/(n-1)))));
    //    }
    //}

    for(int i = 0; i < (int)samp.size();i++){
    nsamp.push_back(samp.at(i)*(0.5*(1-cos(2*M_PI*i/(n-1)))));
    }


    return nsamp;
}

vector<char> BufferMsg::dumb(vector<char> list)
{
    vector<char> tL = list;
    vector<char> fL;
    uint d = list.size();
    int c;

    for(uint i = 0;i<d;i++){
        if(!(i==d-1)){
            if(tL.at(i) == tL.at(i+1)){
                c = 1;
                while(tL.at(i+1)==tL.at(i)){
                    if(i==d-2){
                        i++;
                        c++;
                        break;
                    }
                    i++;
                    c++;
                }
                for (int j = 0; j<c/2;j++){
                    fL.push_back(tL.at(i));
                }
            }else{fL.push_back(tL.at(i));}
        }else{fL.push_back(tL.at(i));}
   }
    return fL;
}


