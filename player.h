#ifndef PLAYER_H
#define PLAYER_H
#include "wall.h"
class Player:public Body
{

public:
	Wall *body;
    Player(Vector3 cen,const float &mass,const Line &rot,const bool&fr,const Vector3 &frt,Body *o);
    ~Player()
    {
		delete body;
    }
    void draw(const Vector3 &color);
    Force getforce(const Vector3 &contactpoint,const float &strength)const;
	void applyforce(const Force &f, const bool &b);
	void collidewithball(Ball &b, const Force &ballforce);
    bool operator ==(const Body *b) const;
    void proceedintime();
    void print()const
    {
		cout <<"player ("<< centerofmass.X() <<","<< centerofmass.Y() <<","<< centerofmass.Z()<<")\n";
    }
};

#endif // PLAYER_H
