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
        turnSpeed = -turnSpeed;
    }

    json j =
        {
        {"linear", {{"x", 0}, {"y", 0}, {"z", 0}}},
        {"angular", {{"x", 0}, {"y", 0}, {"z", turnSpeed}}}
        };

//Vi får dir, som skal definere om det er mod højre eller venstre.
//0 == venstre, 1 == Højre
//Herefter turn, som definere hvor mange grader, i radianer, den skal dreje mod den specificerede side
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
    retning = RobotMovement::retning;
    dist = afstand / 4.775;

    rest = (afstand / 4.775)-dist;

    restfart = (rest - 1.55)/23.1;

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

             json movement;
             movement = Move(retning,0.2);
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

            json movement;

            movement = Turn(RobotMovement::HVvec[0]);

            cout << RobotMovement::vinkelVec[0] << endl;

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
    RobotMovement::koeVec.push_back(x+1);
    RobotMovement::koeVec2.push_back("M");
    RobotMovement::afstandVec.push_back(afstand);
    RobotMovement::retningVec.push_back(retning);
}

void RobotMovement::addTurn(int vinkel, bool retning)
{
    int x = koeVec.size();
    RobotMovement::koeVec.push_back(x+1);
    RobotMovement::koeVec2.push_back("T");
    RobotMovement::vinkelVec.push_back(vinkel);
    RobotMovement::HVvec.push_back(retning);
}

bool RobotMovement::IsConnect()
{
return conn;
}
