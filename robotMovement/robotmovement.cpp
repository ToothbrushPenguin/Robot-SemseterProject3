#include "robotmovement.h"

RobotMovement::RobotMovement()
{

}

RobotMovement::~RobotMovement()
{

}

bool RobotMovement::Move(bool dir, double dist)
{
//Vi skal have testet forskellige hastigheder.
//1. For at kunne finde en sikker hastighed der ikke går får hurtigt.
//2. For at kunne beregne hvor lang tid den skal køre med den hastighed for at nå en hvis længde.
//dir definere om det er forlæns eller bagud, 0 == bagud, 1 == fremad.

}

bool RobotMovement::Turn(bool dir, double turn)
{
//Vi får dir, som skal definere om det er mod højre eller venstre.
//0 == venstre, 1 == Højre
//Herefter turn, som definere hvor mange grader, i radianer, den skal dreje mod den specificerede side
}

bool RobotMovement::Connect()
{
//Denne funktion skal blot connecte til robotten
}

bool RobotMovement::IsConnect()
{
return conn;
}
