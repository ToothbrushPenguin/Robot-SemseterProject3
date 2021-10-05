#include <iostream>
#include "robotmovement.h"
#include "robotmovement.cpp"

using namespace std;

int main()
{
    signal(SIGINT, signalHandler);
    cout << "Initializing for server '" << ADDRESS << "'..." << endl;
    mqtt::async_client cli(ADDRESS, "");
    mqtt::topic top(cli, TOPIC, QOS);

    RobotMovement ex(cli, top);
    bool connected = ex.Connect();
    cout << "Connected: " << connected << endl;

    bool forward = true;
    double speed = 1;
    json move = ex.RobotMovement::Move(forward,speed);

    ex.publishMessage(move);


}
