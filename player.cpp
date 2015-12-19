#include "player.h"
Player::Player(const Vector3 &cen, const double &mass, const Line &rot, const bool &fr, const Vector3 &frt, Body *o)
:Body(mass, cen, rot, fr, frt, o)
, body(new Wall(Player_WIDTH, Player_HEIGHT, Player_DIPTH, mass, cen, rot, fr, frt, this, NULL))
{    
    inertia=body->getinertia();

}

void Player::draw(const Vector3 &color) const
{
    body->draw(color);
}


void Player::applyforce(const Force &f, const bool &b)
{
    Body::applyforce(f, b);
    body->applyforce(f, false);
}

bool Player::operator ==(const Body *b) const
{
    const Player *p=dynamic_cast<const Player*>(b);
    return (p)&&(Body::operator ==(b))&&(p->body==body);
}

void Player::proceedintime()
{
    Body::proceedintime();
    body->proceedintime();
}

void Player::collidewithball(Ball &b, Force &ballforce)
{
	body->collidewithball(b, ballforce);
}

void Player::applytorque(const Force &f, const Vector3 &p, const bool &applyonorigin)
{
    Body::applytorque(f,p,applyonorigin);
    body->applytorque(f,p,false);
}

void Player::applytorque(const Vector3 &torque, const bool &applyonorigin)
{
    Body::applytorque(torque,applyonorigin);
    body->applytorque(torque,false);
}

