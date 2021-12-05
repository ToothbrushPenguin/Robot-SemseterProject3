#include "buffermsg.h"

BufferMsg::BufferMsg()
{
    //test--
    samplefile.open ("samplefile.txt");
    //------
    if(SignalRecorder::isAvailable())
    {
    recorder.start(samplerate);
    this_thread::sleep_for(chrono::milliseconds(2000));
    }

}

vector<char> BufferMsg::recordSignal(int timeout)
{
    int sleeptime = 25;
    int sleepOffset = 0;
    bool toggle = 0;
    int time = 0;


    vector<char> msg = {};
        while(timeout == -1 || time < timeout){

            auto start = chrono::high_resolution_clock::now();

            vector<int> temprec = recorder.getSamp();

            temprecFFT.push_back(temprec);

            for(unsigned int i = 0; i < temprec.size(); i++){
                rec.push_back(temprec.at(i));
            }

            vector<int> fsout = fourierSplit(rec);

            for(int i = 0; i < oldrecLength; i++){
                rec.erase(rec.begin());
            }
            oldrecLength = rec.size();

            cout << result(fsout) << endl;

            if(fsout.size()==2){
                if(toggle == 1&&result(fsout) == '#'){//stop bit
                    return interpreter(msg);
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
                }
                if(result(fsout) == '*'){
                    toggle = 1;
                }
            }
            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(stop-start);
            sleepOffset = duration.count();
            time += sleeptime;
            this_thread::sleep_for(chrono::milliseconds(sleeptime-sleepOffset));

        }
        return {'0'};
}

vector<char> BufferMsg::recordSignalFFT(int timeout)
{
    int sleeptime = 25;
    int sleepOffset = 0;
    bool toggle = 0;
    int time = 0;


    vector<char> msg = {};
        for(uint i = 0; i < temprecFFT.size(); i++){

            auto start = chrono::high_resolution_clock::now();

            vector<int> temprec = temprecFFT[i];

            for(unsigned int i = 0; i < temprec.size(); i++){
                rec.push_back(temprec.at(i));
            }

            vector<int> fsout = fourierSplitFFT(rec);

            for(int i = 0; i < oldrecLength; i++){
                rec.erase(rec.begin());
            }
            oldrecLength = rec.size();

            cout << result(fsout) << endl;

            if(fsout.size()==2){
                if(toggle == 1&&result(fsout) == '#'){//stop bit
                    return interpreter(msg);
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
                }
                if(result(fsout) == '*'){
                    toggle = 1;
                }
            }
            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(stop-start);
            sleepOffset = duration.count();
            time += sleeptime;
            //this_thread::sleep_for(chrono::milliseconds(sleeptime-sleepOffset));

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


vector<int> BufferMsg::fourierSplit(vector<int> samples)
{

    int oriL = samples.size();
    int npad = samplerate - oriL;

    vector<complex<double>> input = {};
    vector<complex<double>> dftout = {};

    vector<int> nsamples;
    nsamples=winFunc(samples);

    for(int i = 0; i < oriL; i++){
        input.push_back((complex<double>)nsamples[i]);
    }

    for(int i = 0; i < npad; i++){
        input.push_back(0.);
    }

    dftout = dft(input);

    vector<double> amps = {};

    for(uint i = 0; i < dftout.size(); i++){
        amps.push_back(abs(dftout[i]));
    }

    int largestIdx = largestInList(amps);
    int largestAmp = amps[largestIdx];
    amps[largestIdx] = 0;
    int secLargestIdx = largestInList(amps);
    int secLargestAmp = amps[secLargestIdx];
    amps[largestIdx] = largestAmp;

    double avgBG = 0;
    for(uint i = 0; i < freqs.size(); i++){
        avgBG += amps[i]/(freqs.size()-2);
    }

    avgBG -= (largestAmp-secLargestAmp)/(freqs.size()-2);

    if(amps[secLargestIdx] > 1.66*avgBG){

        int first = freqs[largestIdx];
        int second = freqs[secLargestIdx];

        if(largestIdx < secLargestIdx && largestIdx < 4 && secLargestIdx >= 4){
            return {first, second};
        }else if(secLargestIdx < largestIdx && secLargestIdx < 4 && largestIdx >= 4){
            return {second, first};
        }else{
            return {0,0};
        }

    }else{
        return {0,0};
    }
}

vector<int> BufferMsg::fourierSplitFFT(vector<int> samples)
{

    //samples = {32767,21071,-25387,-32768,-18824,32767,7210,-32768,-32768,-28911,20949,32767,32767,13794,-31191,-32768,-23090,31727,14138,-32768,-32768,-20837,27773,32767,32767,6259,-32768,-32768,-25919,29574,21572,-29920,-32768,-15324,29802,32767,32767,-1334,-32768,-32768,-27839,25373,32767,-18448,-32768,-8993,31934,32767,32767,-10215,-32768,-32768,-24889,25758,32767,-7115,-32768,-2334,32767,32767,31991,-16137,-32768,-32768,-21213,26804,32767,3455,-32768,3356,32767,32767,25956,-25086,-32768,-32768,-13959,30586,32767,11973,-32768,642,32767,32767,19409,-29954,-32768,-32768,-4031,32767,32767,17113,-32768,-8785,32767,32767,11833,-32418,-32768,-32768,2951,32767,32767,20355,-32768,-19778,30654,32767,3630,-32768,-32768,-32768,12231,32767,32767,20116,-32703,-31843,19652,32767,-1122,-32768,-32768,-30629,17946,32767,32767,16770,-32136,-32768,10749,32767,-7290,-32768,-32768,-24616,26067,32767,32767,10385,-32768,-32768,-854,32767,-8320,-32768,-32768,-17089,31461,32767,32767,387,-32768,-32768,-10579,32767,-4612,-32768,-32768,-10031,32767,32767,32767,-5897,-32768,-32768,-16640,32767,6625,-32768,-32768,-10,32767,32767,32253,-15625,-32768,-32768,-17321,32767,18478,-32415,-32768,6900,32767,32767,27569,-22665,-32768,-32768,-14550,32767,26206,-26788,-32768,13290,32767,32767,20145,-29558,-32768,-32768,-8409,32767,32767,-15299,-32768,15767,32767,32767,14495,-32768,-32768,-32768,982,32767,32767,-3533,-32768,14840,32767,32767,6044,-32768,-32709,-32768,7667,32767,32767,5003,-32768,8420,32767,32767,-2148,-32768,-32768,-32476,15090,32767,32767,10165,-32768,-3197,32767,32767,-11139,-32768,-32768,-26436,24152,32767,32767,8701,-32768,-15985,32767,32767,-15925,-32768,-32768,-20390,29053,32767,32767,2119,-32768,-25623,27101,32767,-20687,-32768,-32768,-11608,32767,32767,32767,-4950,-32768,-32768,13229,32767,-21301,-32768,-32768,-2887,32767,32767,32733,-12105,-32768,-32768,4869,32767,-17678,-32768,-32768,5946,32767,32767,29822,-19647,-32768,-32768,-1028,32767,-10959,-32768,-31199,13227,32767,32767,23003,-27372,-32768,-32768,-648,32767,1290,-32768,-28250,20619,32767,32767,17826,-30909,-32768,-32768,1684,32767,14305,-32768,-27045,25285,32767,32767,8859,-32768,-32768,-32768,5455,32767,24865,-29132,-24658,29532,32767,32767,-1185,-32768,-32768,-30947,14423,32767,31546,-18158,-30936,24516,32767,32767,-7568,-32768,-32768,-28161,20711,32767,32767,-10207,-31437,23287,32767,30211,-15308,-32768,-32768,-21669,28569,32767,32767,-4137,-32768,14253,32767,26443,-24906,-26672,30295,32767,-10253,-32768,-30931};
    vector<double> chanceoffrek = {};
    vector<double> avchanceoffrek = {};
    int oriL = samples.size();
    int npad = samplerate - oriL;
    int padSide = 10;
    int Fn = 1700;

    vector<complex<double>> input = {};
    vector<complex<double>> fft = {};

    vector<int> nsamples;
    nsamples=winFunc(samples);

    for(int i = 0; i < oriL; i++){
        input.push_back((complex<double>)nsamples[i]);
    }

    for(int i = 0; i < npad; i++){
        input.push_back(0.);
    }

    fft = fastFourier(input);

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

    return twoLargest(avchanceoffrek);
}

vector<complex<double> > BufferMsg::fastFourier(vector<complex<double>> msg)
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
    }

    vector<complex<double>> ye = fastFourier(pe);
    vector<complex<double>> yo = fastFourier(po);

    complex<double> w(cos(-2*M_PI/n), sin(-2*M_PI/n));
    vector<complex<double>> y(n,0);

    for(unsigned int i = 0; i < n/2; i++){
        y[i] = ye[i]+(pow(w,i)*yo[i]);
        y[i+(n/2)] = ye[i]-(pow(w,i)*yo[i]);
    }

    return y;
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

    int largestIDXL = largestInList(eightL);
    int largestIDXH = largestInList(eightH);

    if((eightL[largestIDXL]>avgL*lowerRatio && eightL[largestIDXL]>lowerBound) && (eightH[largestIDXH]>avgH*lowerRatio && eightH[largestIDXH]>lowerBound)){

        return{eightLFrek[largestIDXL],eightHFrek[largestIDXH]};
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

vector<complex<double>> BufferMsg::dft(vector<complex<double>> input)
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

int BufferMsg::largestInList(vector<double> list)
{
    //finding the largest value of a list and return the index of it
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

vector<int> BufferMsg::winFunc(vector<int> samp)
{
    vector<int> nsamp={};
    int n=samp.size();

    for(int i = 0; i < (int)samp.size();i++){
    nsamp.push_back(samp.at(i)*(0.5*(1-cos(2*M_PI*i/(n-1)))));
    }

    return nsamp;
}

vector<char> BufferMsg::interpreter(vector<char> list)
{
    vector<char> totalList = list;
    vector<char> finalList;
    uint listLength = list.size();
    int count;

    for(uint i = 0;i<listLength;i++){
        if(!(totalList[i]=='N')){
            if(!(i==listLength-1)){
                if(totalList.at(i) == totalList.at(i+1)){
                    count = 1;
                    while(totalList.at(i+1)==totalList.at(i)){
                        if(i==listLength-2){
                            i++;
                            count++;
                            break;
                        }
                        i++;
                        count++;
                    }
                    for(int j = 0; j<round((double)count/2);j++){
                    //for(int j = 0; j<count/2;j++){
                        finalList.push_back(totalList.at(i));
                    }
                }else{finalList.push_back(totalList.at(i));}
            }else{finalList.push_back(totalList.at(i));}
        }
    }
    return finalList;
}




