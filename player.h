#ifndef PLAYER_H
#define PLAYER_H
#include "wall.h"
class Player:public Body
{

public:
	Wall *body;
    Player(const Vector3 &cen,const float &mass,const Line &rot,const bool&fr,const Vector3 &frt,Body *o);
    ~Player()
    {
		delete body;
    }
    void draw(const Vector3 &color)const;
    Force getforce(const Vector3 &contactpoint,const float &strength)const;
    void applyforce(const Force &f, const bool &b);

    void collidewithball(Ball &b, Force &ballforce);
    bool operator ==(const Body *b) const;
    void proceedintime();        

    void applytorque(const Force &f, const Vector3 &p, const bool &applyonorigin);
    void applytorque(const Vector3 &torque, const bool &applyonorigin);
    void stopRotation(const bool &applyonorigin);
    void stopMovement(const bool &applyonorigin);
};

#endif // PLAYER_H
