#include "msghandeler.h"


MsgHandeler::MsgHandeler()
{

}

void MsgHandeler::handshake(bool in)
{
    MsgBuffer buf;
    sf::SoundBuffer sBuf;
    sf::Sound sound;
    vector<char> truFal = {'*'};

    vector<char> inc;
    if (in == true){
        inc ={'a','b','1'};
    }else if (in == false){
        inc ={'a','b','0'};
    }
    vector<char> incs = incoder(inc);
    for (uint i = 0;incs.size();i++){
        truFal.push_back(incs.at(i));
    } truFal.push_back('#');
    sBuf = buf.convert({truFal});
    sound.setBuffer(sBuf);
    sound.play();
    while(sound.getStatus()==2){
    }
}

State MsgHandeler::isStartStop(vector<char> in)
{
    string stop = "ad";
    string curStr;
    curStr.push_back(in.at(0));
    curStr.push_back(in.at(1));

    if (curStr==stop){
        return STOP;
    }
    return RUNNING;
}

Direction MsgHandeler::DecodeMovement(vector<char> in)
{
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
        dir = HALT;
    }
    return dir;
}

double MsgHandeler::decodeValue(vector<char> in)
{
    double out=0;
        for(unsigned int i = 0; i < in.size()-8; i++){
            out+=(in.at(in.size()-7-i)-48)*pow(10,i);
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
