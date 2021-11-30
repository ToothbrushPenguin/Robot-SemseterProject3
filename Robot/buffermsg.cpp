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
    int sleeptime = 25;
    int sleepOffset = 0;
    bool toggle = 0;
    int time = 0;
    vector<char> msg = {};
        while(timeout == -1 || time < timeout){

            auto start = chrono::high_resolution_clock::now();

            vector<int> temprec = recorder.getSamp();

            for(unsigned int i = 0; i < temprec.size(); i++){
                rec.push_back(temprec.at(i));
            }

            fsout = FourierSplit(rec);

            for(int i = 0; i < oldrecLength; i++){
                rec.erase(rec.begin());
            }
            oldrecLength = rec.size();

            cout << result(fsout) << endl;

            if(fsout.size()==2){
                if(toggle == 1&&result(fsout) == '#'){//stop bit
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

BufferMsg::~BufferMsg()
{
    //test---
    samplefile.close();
    //----
    recorder.stop();
}


vector<int> BufferMsg::FourierSplit(vector<int> samples)
{
    vector<double> freqs = {697, 770, 852, 941, 1209, 1336, 1477, 1633};

    int oriL = samples.size();
    int npad = samplerate - oriL;

    vector<complex<double>> input = {};
    vector<complex<double>> dft = {};

    vector<int> nsamples;
    nsamples=winFunc(samples);

    for(int i = 0; i < oriL; i++){
        input.push_back((complex<double>)nsamples[i]);
    }

    for(int i = 0; i < npad; i++){
        input.push_back(0.);
    }

    dft = DFT(input);

    vector<double> amps = {};

    for(uint i = 0; i < dft.size(); i++){
        amps.push_back(abs(dft[i]));
    }

    int largestIdx = LargestInList(amps);
    int largestAmp = amps[largestIdx];
    amps[largestIdx] = 0;
    int secLargestIdx = LargestInList(amps);
    int secLargestAmp = amps[secLargestIdx];
    amps[largestIdx] = largestAmp;

    double avgBG = 0;
    for(uint i = 0; i < freqs.size(); i++){
        avgBG += amps[i]/(freqs.size()-2);
    }

    avgBG -= (largestAmp-secLargestAmp)/(freqs.size()-2);

    if(amps[secLargestIdx] > 2.5*avgBG){

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

vector<complex<double>> BufferMsg::DFT(vector<complex<double>> input)
{

    int N = input.size();
    vector<double> freqs = {697, 770, 852, 941, 1209, 1336, 1477, 1633};
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

int BufferMsg::LargestInList(vector<double> list)
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

vector<char> BufferMsg::dumb(vector<char> list)
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




