#include "msghandeler.h"


MsgHandeler::MsgHandeler()
{

}

void MsgHandeler::handshake(bool crc, int readPn)
{
    MsgBuffer buf;
    sf::SoundBuffer sBuf;
    sf::Sound sound;
    vector<char> truFal = {'*'};

    vector<char> inc;
    if(crc){
        inc ={'a','b', (char)(readPn+48)};
    }else{
        inc ={'a','b','0'};
    }
    vector<char> incs = crcIncoder(inc);
    for (uint i = 0; i < incs.size(); i++){
        truFal.push_back(incs.at(i));
    } truFal.push_back('#');

    sBuf = buf.convert({truFal});
    sound.setBuffer(sBuf);
    sound.play();
    while(sound.getStatus()==2){
    }
}

State MsgHandeler::isStop(vector<char> in)
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
        for(unsigned int i = 0; i < in.size()-10; i++){
            out+=(in.at(in.size()-8-i)-48)*pow(10,i);
        }
    return out;

}

vector<char> MsgHandeler::crcIncoder(vector<char> in)
{
    int number;
    unsigned long int value=0;
    unsigned long divider = 65521;
    for(unsigned int i = 1; i < in.size()+1;i++){
        switch (in[in.size()-i]) {
        case 'a':number = 10;break;
        case 'b':number = 11;break;
        case 'c':number = 12;break;
        case 'd':number = 13;break;
        default:number = in[in.size()-i]-48;break;
        }

        value += number*pow(16,i-1);
    }
    value *= 100000;
    unsigned long mod = value % divider;
    unsigned long end = divider - mod;

    stringstream ss;
    ss<<end;
    string s;
    ss>>s;

    int zeros = 0;
    for(unsigned int i = 0; i < s.size(); i++){
        if((int)s.size() < 5 - zeros){
            in.push_back('0');
            zeros++;
            i--;
        }else{
            in.push_back(s.at(i));
        }
    }
    return in;

}

vector<char> MsgHandeler::seqIncoder(vector<char> msg, int pnIn)
{

    vector<char> msgout = msg;
    msgout.push_back('a');
    msgout.push_back('b');
    msgout.push_back((char)(pnIn+48));

    return msgout;
}

vector<char> MsgHandeler::ssbit(vector<char> msg)
{
    vector<char> out = {'*'};
    for(unsigned int i = 0; i < msg.size(); i++){
        out.push_back(msg[i]);
    }
    out.push_back('#');

    return out;
}

bool MsgHandeler::isValid(vector<char>in)
{
    int number;
    unsigned long front = 0;
    unsigned long back = 0;
    unsigned long value=0;
    unsigned long divider = 65521;

    if(in.size()==1 && in[0] == '0'){
        return 0;
    }

    for(unsigned int i = 0; i < in.size()-5; i++){
        switch (in[i]) {
        case 'a':number = 10;break;
        case 'b':number = 11;break;
        case 'c':number = 12;break;
        case 'd':number = 13;break;
        default:number = in[i]-48;break;
        }
        front += number*pow(16,in.size()-5-i-1);
    }

    for(unsigned int i = 0; i < 5; i++){
        int backn = in[in.size()-i-1]-48;
        back += backn*pow(10,i);
    }
    value = front*100000+back;
    if(value % divider == 0){
        return 1;
    }
    return 0;
}

int MsgHandeler::readPn(vector<char> msg)
{
    int pnout = 0;
    for(unsigned int i = 1; i < msg.size()-5; i++){
        if(msg[i-1] == 'a' && msg [i] == 'b'){
            pnout = ((int)msg[i+1]-48);
        }
    }
    return pnout;
}

bool MsgHandeler::correctPn(int readpn)
{
    MsgHandeler han;
    if(readpn == robPn + 1){
        han.incRobPn();
        return true;
    }
    return false;
}

void MsgHandeler::resetRobPn()
{
    robPn = 1;
}

void MsgHandeler::incRobPn()
{
    if(robPn == 9){
        resetRobPn();
    }else{
        robPn++;
    }
}

int MsgHandeler::getRobPn()
{
    return robPn;
}
