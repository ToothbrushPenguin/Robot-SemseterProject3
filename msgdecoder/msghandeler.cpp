#include "msghandeler.h"


MsgHandeler::MsgHandeler()
{

}

vector<char> MsgHandeler::decodeAndRun(vector<char> in)
{

}

Direction MsgHandeler::DecodeMovement(vector<char> in)
{
    if (isValid(in)){
        Direction dir;
        string msgDir;
        msgDir.push_back(in.at(0));
        msgDir.push_back(in.at(1));
        if(msgDir == "aa"){
            dir = LEFT;
        }else if(msgDir == "bb"){
            dir = RIGHT;
        }else if(msgDir == "cc"){
            dir = UP;
        }else if(msgDir == "dd"){
            dir = DOWN;
        }else{
            dir = STOP;
        }
        return dir;
    }
    return STOP;



}

double MsgHandeler::decodeValue(vector<char> in)
{
    double out=0;
    if (isValid(in)){
        for(unsigned int i = 0; i < in.size()-8; i++){
            out+=(in.at(in.size()-7-i)-48)*pow(10,i);
        }
    }
    return out;

}

vector<char> MsgHandeler::incoder(vector<char> in)
{
    string s;
    for(unsigned int i = 0; i < in.size(); i++){
        s.push_back(in.at(i));
    }
    unsigned long value=0x0;
    istringstream iss(s);
    iss >> hex >> value;
    value*=0x100000;
    unsigned long divider =0xfff1;
    unsigned long mod = value % divider;
    unsigned long end = divider - mod;
    unsigned long finalMsg = value + end;

    ostringstream oss;
    oss<<hex<<finalMsg;
    s = oss.str();

    vector<char> out;
    for(unsigned int i = 0; i < s.size(); i++){
        out.push_back(s.at(i));
    }
    return out;

}

bool MsgHandeler::isValid(vector<char>in)
{
    string s;
    for(unsigned int i = 0; i < in.size(); i++){
        s.push_back(in.at(i));
    }
    unsigned long value;
    istringstream iss(s);
    iss >> hex >> value;
    if(value % 0xfff1 == 0){
        return 1;
    }
    return 0;
}
