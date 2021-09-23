#include "buffermsg.h"

BufferMsg::BufferMsg()
{
}

vector<char> BufferMsg::SignalRecord()
{    
    bool toggle = 0;
    vector<char> msg = {};

    if (SignalRecoder::isAvailable())
    {
        while(true){
            recorder.start(samplerate);
            this_thread::sleep_for(chrono::milliseconds(100));
            recorder.stop();
            fsout = FourierSplit(recorder.getSampels());

            if(fsout.size()==2){
                if(result(fsout) == '*'||toggle == 1){
                    if((result(fsout) == '*' && toggle == 1)||result(fsout) == 'N'){

                    }else{
                        toggle = 1;
                        msg.push_back(result(fsout));
                    }
                }
                if(result(fsout) == '#'){//stop bit
                    return msg;
                }
            }
        }
    }
    return {'0'};
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
    double first4difrence =first4max-*min_element(first4low.begin(),first4low.end());
    double larst4difrence =larst4max-*min_element(larst4high.begin(),larst4high.end());

    int lowfrek=0;
    int highfrek=0;

    if(first4difrence >= 45 && larst4difrence >= 45){

        for(unsigned int u = 0; u < first4low.size(); u++){
            if(first4low[u]==first4max){
                lowfrek=(int)frequencys[u];
            }
            if(larst4high[u]==larst4max){
                highfrek=(int)frequencys[u+4];
            }
        }

    }

    return {lowfrek,highfrek};
}

char BufferMsg::result(vector<int> frequency)
{

    vector<int> n;
    if(frequency[0]!=0&&frequency[1]!=0){
        switch(frequency[0])
        {
        case 697:n={'1','2','3','A'} ; break;
        case 770:n={'4','5','6','B'} ; break;
        case 852:n={'7','8','9','C'} ; break;
        case 941:n={'*','0','#','D'} ; break;
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




