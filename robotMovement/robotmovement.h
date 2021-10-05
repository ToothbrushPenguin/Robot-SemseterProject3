#ifndef ROBOTMOVEMENT_H
#define ROBOTMOVEMENT_H


class RobotMovement
{
public:
    RobotMovement();
    ~RobotMovement();
    bool Move(bool dir, double  dist);
    bool Turn(bool dir, double turn);
    //void Disconnect();

private:
    bool Connect();
    bool IsConnect();
    bool conn = false;
    double speed;
    double turnDir;
};

#endif // ROBOTMOVEMENT_H
