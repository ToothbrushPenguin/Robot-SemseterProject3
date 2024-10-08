#include "msghandler.h"

MsgHandler::MsgHandler()
{

}

void MsgHandler::handshake(bool crc, int readPn)
{
    MsgBuffer buf;
    sf::SoundBuffer sBuf;
    sf::Sound sound;
    vector<char> truFal = {};

    vector<char> inc;
    if(crc){
        inc = {*(to_string(readPn).c_str())};
    }else{
        inc ={'0'};
    }
    cout << "readPn: " << readPn << endl;
    cout << "Ack number: " << inc[0] << endl;
    truFal = ssbit(crcIncoder(inc));
    truFal.push_back('#');
    cout << "robPn: " << robPn << endl;



    for(unsigned int u = 0; u< truFal.size(); u++){
        sBuf = buf.convert(truFal[u]);

        sound.setBuffer(sBuf);

        sound.play();

        while(sound.getStatus()==2){
        }
    }
}

State MsgHandler::isStop(vector<char> in)
{
    string stop = "abc";
    string curStr;
    curStr.push_back(in.at(0));
    curStr.push_back(in.at(1));
    curStr.push_back(in.at(2));

    if (curStr==stop){
        return STOP;
    }
    return RUNNING;
}

Direction MsgHandler::decodeMovement(vector<char> in)
{
    Direction dir;
    string msgDir;
    msgDir.push_back(in.at(0));
    if(msgDir == "a"){
        dir = LEFT;
    }else if(msgDir == "b"){
        dir = RIGHT;
    }else if(msgDir == "c"){
        dir = UP;
    }else if(msgDir == "d"){
        dir = DOWN;
    }else{
        dir = HALT;
    }
    return dir;
}

double MsgHandler::decodeValue(vector<char> in)
{
    double out=0;
        for(unsigned int i = 0; i < in.size()-7; i++){
            out+=(in.at(in.size()-7-i)-48)*pow(10,i);
        }
    return out;

}

vector<char> MsgHandler::crcIncoder(vector<char> in)
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

vector<char> MsgHandler::seqIncoder(vector<char> msg, int pnIn)
{

    vector<char> msgout = msg;
    msgout.push_back((char)(pnIn+48));

    return msgout;
}

vector<char> MsgHandler::ssbit(vector<char> msg)
{
    vector<char> out = {'*'};
    for(unsigned int i = 0; i < msg.size(); i++){
        out.push_back(msg[i]);
    }
    out.push_back('#');

    return out;
}

bool MsgHandler::isValid(vector<char>in)
{
    int number;
    unsigned long front = 0;
    unsigned long back = 0;
    unsigned long value=0;
    unsigned long divider = 65521;

    if((in.size()==1 && in[0] == '0') || (in.size() < 6)){
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

int MsgHandler::readPn(vector<char> msg)
{
    if(msg.size()>5){
        int pnout = ((int)msg[msg.size()-6]-48);
        return pnout;
    }else{
        return 0;
    }

}

bool MsgHandler::correctPn(int readpn)
{
    if((readpn == robPn + 1)||(readpn == 9 && robPn == 0)){
        return true;
    }
    return false;
}

void MsgHandler::resetRobPn()
{
    robPn = 0;
}

void MsgHandler::incRobPn()
{
    if(robPn >= 8){
        resetRobPn();
    }else{
        robPn++;
    }
}

int MsgHandler::getRobPn()
{
    return robPn;
}
