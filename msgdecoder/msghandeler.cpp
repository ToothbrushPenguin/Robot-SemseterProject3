#include "msghandeler.h"

MsgHandeler::MsgHandeler()
{

}

vector<char> MsgHandeler::decodeAndRun(vector<char> e)
{

}

Direction MsgHandeler::DecodeMovement(vector<char>)
{

}

double MsgHandeler::decodeValue(vector<char>)
{

}

bool MsgHandeler::isValid(vector<char>in)
{
    unsigned int sumIndex=0;
    int sum1 = 0;
    int sum2 = 0;
    for(unsigned int i = 0; i <= in.size(); i++){
        if(i<in.size()){
            if(in.at(i)=='A'&&in.at(i+1)=='D'){
            sumIndex=i+2;
            }
        }
    }
    if(sumIndex==0){
        return 0;
    }
    for(unsigned int i = 0; i < in.size()-sumIndex; i++){
        stringstream str;
        str<<in.at(sumIndex+i);
        int s;
        str>>s;
        sum1+=s*pow(10,in.size()-sumIndex-1-i);
    }
    for(unsigned int i = 0; i <= sumIndex-3; i++){
        if(in.at(i)=='1'||'2'||'3'||'4'||'5'||'6'||'7'||'8'||'9'){
            stringstream str;
            str<<in.at(i);
            int s;
            str>>s;
            sum2+=s;
        }
        switch (in.at(i)) {
        case'A':sum2+=20;break;
        case'B':sum2+=30;break;
        case'C':sum2+=40;break;
        case'D':sum2+=50;break;
        case'0':sum2+=110;break;
        default:break;
        }
    }
    if(sum1==sum2){
       return 1;
    }else {
       return 0;
    }
}
