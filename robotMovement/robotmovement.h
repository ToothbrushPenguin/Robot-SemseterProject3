#ifndef ROBOTMOVEMENT_H
#define ROBOTMOVEMENT_H


class RobotMovement
{
public:
    RobotMovement();
    ~RobotMovement();
    void move();
    void turn(bool dir, float turn);

private:
    void setSpeed();
    void setTurn();
    void connect();
    bool isConnect();
    bool conn = false;
    float speed;
    float turnDir;
};

#endif // ROBOTMOVEMENT_H
