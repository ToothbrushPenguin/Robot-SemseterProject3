#include "robotmovement.h"

using namespace std;

RobotMovement::RobotMovement(mqtt::async_client& client, mqtt::topic& topic)
{
    cli = &client;
    top = &topic;
}

RobotMovement::~RobotMovement()
{

}

json RobotMovement::Move(bool dir, double speed)
{
    if(dir == false)
    {
        speed = -speed;
    }

    json j =
        {
        {"linear", {{"x", speed}, {"y", 0}, {"z", 0}}},
        {"angular", {{"x", 0}, {"y", 0}, {"z", 0}}}
        };
    return j;
}
json RobotMovement::Turn(bool dir)
{

    if(dir == true)
    {
        RobotMovement::turnSpeed = -RobotMovement::turnSpeed;
    }

    json j =
        {
        {"linear", {{"x", 0}, {"y", 0}, {"z", 0}}},
        {"angular", {{"x", 0}, {"y", 0}, {"z", RobotMovement::turnSpeed}}}
        };
    if(dir == true)
    {
        RobotMovement::turnSpeed = -RobotMovement::turnSpeed;
    }
    return j;
}

void RobotMovement::publishMessage(json j)
{
        try {
            tok = top->publish(j.dump());
            tok->wait();
        }
        catch (const mqtt::exception& exc) {
            cerr << exc << endl;
            return;
        }
}

bool RobotMovement::Connect() // Connecter robotten og outputter i terminal
{
    try {
        cout << "\nConnecting..." << endl;
        cli->connect()->wait();
        cout << "  ...OK" << endl;
    }
    catch (const mqtt::exception& exc) {
        cerr << exc << endl;
        return false;
    }
    return true;
}

void RobotMovement::UdregningMove(int afstand, bool retning)
{

    RobotMovement::retning = retning;
    dist = afstand / 4.775;         //dist is the amount of times the send message is gonna loop (231*0,2+1,55)/10 = 4,775 (dist is an integer)

    rest = (afstand / 4.775)-dist;  //rest, is how much of the distance is left to achieve the desired distance

    restfart = (rest - 1.55)/23.1;  //restfart is what speed we need to set the motors to, for it to drive the last distance.
                                    //this make the equation so that the speed is a function of the last distance instead of the distance being based on speed.

    if(restfart < 0)
    {
        restfart = -restfart;
    }

}

void RobotMovement::UdregningTurn(int vinkel)
{

    vinkel = RobotMovement::antalOmdrejninger;
}

void RobotMovement::sendMovement()
{

    for(uint i = 0; i < koeVec.size(); i++)
    {
        if(koeVec2[i] == "M")
        {
             UdregningMove(RobotMovement::afstandVec[0],RobotMovement::retningVec[0]);

             cout << "Kører: " <<RobotMovement::afstandVec[0] << endl;
             cout << "drejer: " << RobotMovement::retningVec[0] << endl;
             cout << "move retning: " << RobotMovement::retning << endl;
             cout << endl;

             json movement;
             movement = Move(retning,speed);    //using defined speed (0,2)
             for(int i = 0; i < dist; i++)
             {
                 publishMessage(movement);
                 this_thread::sleep_for(chrono::milliseconds(200));
             }

             movement = Move(retning,restfart);
             publishMessage(movement);
             this_thread::sleep_for(chrono::milliseconds(200));

             afstandVec.erase(afstandVec.begin());
             retningVec.erase(retningVec.begin());
        }
        else if(koeVec2[i] == "T")
        {
            UdregningTurn(RobotMovement::vinkelVec[0]);
            cout << "Drejer: " <<RobotMovement::vinkelVec[0] << endl;
            cout << "retning: " << RobotMovement::HVvec[0] << endl;
            cout << endl;

            json movement;

            movement = Turn(RobotMovement::HVvec[0]);

            for(int i = 0; i < RobotMovement::vinkelVec[0]; i++)
            {
                publishMessage(movement);
                this_thread::sleep_for(chrono::milliseconds(500));
            }

            vinkelVec.erase(vinkelVec.begin());
            HVvec.erase(HVvec.begin());
        }
    }

    RobotMovement::afstandVec.clear();
    RobotMovement::retningVec.clear();
    RobotMovement::vinkelVec.clear();
    RobotMovement::HVvec.clear();
    RobotMovement::koeVec.clear();
    RobotMovement::koeVec2.clear();

    return;

}

void RobotMovement::addMovement(int afstand, bool retning)
{
    int x = koeVec.size();
    RobotMovement::koeVec.push_back(x+1);       //index of the specefic move added
    RobotMovement::koeVec2.push_back("M");      //M for Move
    RobotMovement::afstandVec.push_back(afstand);//distance to move
    RobotMovement::retningVec.push_back(retning);//Forward or backwards
}

void RobotMovement::addTurn(int vinkel, bool retning)
{
    int x = koeVec.size();
    RobotMovement::koeVec.push_back(x+1);       //index of the specific turn added.
    RobotMovement::koeVec2.push_back("T");      //T for Turn
    RobotMovement::vinkelVec.push_back(vinkel); //How much to turn
    RobotMovement::HVvec.push_back(retning);    //Left or Right turn
}

bool RobotMovement::IsConnect()
{
return conn;
}
