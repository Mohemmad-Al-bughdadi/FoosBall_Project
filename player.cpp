#include "player.h"
Player::Player(Vector3 cen, const float &mass, const Line &rot, const bool &fr, const Vector3 &frt, Body *o)
:Body(mass, cen, rot, fr, frt, o)
, body(new Wall(Player_WIDTH, Player_HEIGHT, Player_DIPTH, mass, cen, rot, fr, frt, this, NULL))
{    
    inertia=body->getinertia();

}

void Player::draw(const Vector3 &color)
{
    glPushMatrix();
    glColor3fv(color.toArray());
	Vector3 b = rotationaxis.getBegin();
	Vector3 e = rotationaxis.getEnd() - b;
	// glTranslatef(-b.X(),-b.Y(),-b.Z());
	// glRotatef(thetarotate*180/PI,e.X(),e.Y(),e.Z());
	// glTranslatef(b.X(),b.Y(),b.Z());
	glColor3fv(color.toArray());
	//glTranslated(centerofmass.X(), centerofmass.Y(), centerofmass.Z());
	body->draw(color);

    glPopMatrix();
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

void Player::collidewithball(Ball &b, const Force &ballforce)
{
	body->collidewithball(b, ballforce);
}



