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

double MsgHandeler::decodeValue(vector<char> in)
{
    vector<char> iso;
    for(unsigned int i = 2; i < in.size()-6; i++){
       iso.push_back(in.at(i));
    }

}

vector<char> MsgHandeler::incoder(vector<char> in)
{
    string s;
    for(unsigned int i = 0; i < in.size(); i++){
        s.push_back(in.at(i));
    }
    long value;
    istringstream iss(s);
    iss >> hex >> value;
    value*=0x100000;
    long divider =0xfff1;
    long mod = value % divider;
    long end = divider - mod;
    long finalMsg = value + end;

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
