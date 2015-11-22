#include "Handle.h"
#include "player.h"
#define ROLL_BALL radius+0.1



Handle::Handle(const Handle_Type &thetype, const float &handle_mass, const Vector3 &handle_center,const Line &handle_rotationaxis,const float &r,const bool &fr,const Vector3 &frt,Body *o)
:Body(handle_mass, handle_center, handle_rotationaxis,fr,frt,o), Htype(thetype), radius(r)
{    
    if (thetype == goalkeeper)
    {

        balls.push_back(new Ball(ROLL_BALL, handle_center + Vector3(ROLL_BALL, 0, 0), 10,handle_rotationaxis,false,frt, 0,this));
        players.push_back(new Player(handle_center + Vector3(ROLL_BALL+Player_WIDTH, 0, 0), 10, handle_rotationaxis,false,frt, this));
		players[0]->print();
        balls.push_back(new Ball(ROLL_BALL, handle_center + Vector3(ROLL_BALL+(Player_WIDTH*2), 0, 0), 10,handle_rotationaxis,false,frt, 0,this));
    }

    else if (thetype == defenser)
    {
        balls.push_back(new Ball(ROLL_BALL, handle_center + Vector3(-GOAL_WIDTH / 2 + ROLL_BALL, 0, 0), 10,handle_rotationaxis,false,frt, 0,this));
        players.push_back(new Player(handle_center + Vector3(-GOAL_WIDTH / 2 + ROLL_BALL + Player_WIDTH, 0, 0), 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center + Vector3(-GOAL_WIDTH / 2 + ROLL_BALL + (Player_WIDTH * 4), 0, 0), 10, handle_rotationaxis,false,frt, this));
        balls.push_back(new Ball(ROLL_BALL, handle_center + Vector3(-GOAL_WIDTH / 2 + ROLL_BALL + (Player_WIDTH * 5), 0, 0), 10,handle_rotationaxis,false,frt, 0,this));

	}
	else if (thetype == enemygoalkeeper)
	{
		balls.push_back(new Ball(ROLL_BALL, handle_center - Vector3(ROLL_BALL, 0, 0), 10, handle_rotationaxis, false, frt, 0, this));
		players.push_back(new Player(handle_center - Vector3(ROLL_BALL + Player_WIDTH, 0, 0), 10, handle_rotationaxis, false, frt, this));
		balls.push_back(new Ball(ROLL_BALL, handle_center - Vector3(ROLL_BALL + (Player_WIDTH * 2), 0, 0), 10, handle_rotationaxis, false, frt, 0, this));


	}
    else if (thetype == enemydefenser)
	{
        balls.push_back(new Ball(ROLL_BALL, handle_center - Vector3(-GOAL_WIDTH / 2+ROLL_BALL, 0, 0), 10,handle_rotationaxis,false,frt, 0,this));
        players.push_back(new Player(handle_center - Vector3(-GOAL_WIDTH / 2 + ROLL_BALL + Player_WIDTH, 0, 0), 10,handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center - Vector3(-GOAL_WIDTH / 2 + ROLL_BALL + (Player_WIDTH * 4), 0, 0), 10, handle_rotationaxis,false,frt, this));
        balls.push_back(new Ball(ROLL_BALL, handle_center - Vector3(-GOAL_WIDTH / 2 + ROLL_BALL + (Player_WIDTH * 5), 0, 0), 10,handle_rotationaxis,false,frt, 0,this));

	}
    else if (thetype == attacker)
	{
        balls.push_back(new Ball(ROLL_BALL, handle_center + Vector3(ROLL_BALL, 0, 0), 10,handle_rotationaxis,false,frt, 0,this));
        players.push_back(new Player(handle_center + Vector3(ROLL_BALL + Player_WIDTH*2, 0, 0), 10,handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center + Vector3(ROLL_BALL + (Player_WIDTH * 4), 0, 0), 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center + Vector3(ROLL_BALL + (Player_WIDTH * 6), 0, 0), 10, handle_rotationaxis,false,frt, this));
        balls.push_back(new Ball(ROLL_BALL, handle_center + Vector3(ROLL_BALL + (Player_WIDTH * 8), 0, 0), 10,handle_rotationaxis,false,frt, 0,this));

    }
    else if (thetype == enemyattacker)
	{
        balls.push_back(new Ball(ROLL_BALL, handle_center - Vector3(ROLL_BALL, 0, 0), 10,handle_rotationaxis,false,frt, 0,this));
        players.push_back(new Player(handle_center - Vector3(ROLL_BALL + Player_WIDTH * 2, 0, 0), 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center - Vector3(ROLL_BALL + (Player_WIDTH * 4), 0, 0), 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center - Vector3(ROLL_BALL + (Player_WIDTH * 6), 0, 0), 10, handle_rotationaxis,false,frt, this));
        balls.push_back(new Ball(ROLL_BALL, handle_center - Vector3(ROLL_BALL + (Player_WIDTH * 8), 0, 0), 10,handle_rotationaxis,false,frt, 0,this));

    }
    else if (thetype == mider)
	{
        balls.push_back(new Ball(ROLL_BALL, handle_center + Vector3(-TABLE_WIDTH / 4 + ROLL_BALL, 0, 0), 10,handle_rotationaxis,false,frt, 0,this));
        players.push_back(new Player(handle_center + Vector3(-TABLE_WIDTH / 4 + ROLL_BALL + Player_WIDTH * 3, 0, 0), 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center + Vector3(-TABLE_WIDTH / 4 + ROLL_BALL + (Player_WIDTH * 5), 0, 0), 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center + Vector3(-TABLE_WIDTH / 4 + ROLL_BALL + (Player_WIDTH * 7), 0, 0), 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center + Vector3(-TABLE_WIDTH / 4 + ROLL_BALL + (Player_WIDTH * 9), 0, 0), 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center + Vector3(-TABLE_WIDTH / 4 + ROLL_BALL + (Player_WIDTH * 11), 0, 0), 10, handle_rotationaxis,false,frt, this));

        balls.push_back(new Ball(ROLL_BALL, handle_center + Vector3(-TABLE_WIDTH / 4 + ROLL_BALL + (Player_WIDTH * 13), 0, 0), 10,handle_rotationaxis,false,frt, 0,this));

    }
    else if (thetype == enemymider)
	{
        balls.push_back(new Ball(ROLL_BALL, handle_center - Vector3(-TABLE_WIDTH / 4 +ROLL_BALL, 0, 0), 10,handle_rotationaxis,false,frt, 0,this));
        players.push_back(new Player(handle_center - Vector3(-TABLE_WIDTH / 4 + ROLL_BALL + Player_WIDTH * 3, 0, 0), 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center - Vector3(-TABLE_WIDTH / 4 + ROLL_BALL + (Player_WIDTH * 5), 0, 0), 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center - Vector3(-TABLE_WIDTH / 4 + ROLL_BALL + (Player_WIDTH * 7), 0, 0), 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center - Vector3(-TABLE_WIDTH / 4 + ROLL_BALL + (Player_WIDTH * 9), 0, 0), 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center - Vector3(-TABLE_WIDTH / 4 + ROLL_BALL + (Player_WIDTH * 11), 0, 0), 10, handle_rotationaxis,false,frt, this));

        balls.push_back(new Ball(ROLL_BALL, handle_center - Vector3(-TABLE_WIDTH / 4 + ROLL_BALL + (Player_WIDTH * 13), 0, 0), 10,handle_rotationaxis,false,frt, 0,this));

    }
    float in=0.0f;
	for (unsigned int i = 0; i<players.size(); i++)
        in+=players[i]->getinertia();
	for (unsigned int i = 0; i<balls.size(); i++)
        in+=balls[i]->getinertia();
    inertia=(1.2f*handle_mass*radius*radius)+in;
}
bool Handle::operator ==(const Body *b) const
{
    const Handle *h=dynamic_cast<const Handle *>(b);
    return (h)&&(Body::operator ==(b))&&(h->Htype==Htype);
}

void Handle::proceedintime()
{
    Body::proceedintime();
	for (unsigned int i = 0; i<players.size(); i++)
        players[i]->proceedintime();
	for (unsigned int i = 0; i<balls.size(); i++)
        balls[i]->proceedintime();
}

void Handle::fillconstants(Wall *w) const
{
	for (unsigned int i = 0; i<balls.size(); i++)
    {
         Body::StaticFrictionCoffeciants.insert(pair<BodyPair,float>(BodyPair(balls[i],w),0.01f));
         Body::RestCoffeciants.insert(pair<BodyPair,float>(BodyPair(balls[i],w),0.01f));
		 Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(balls[i], w), 0.01f));
    }
}

void Handle::collidewallwithballs(Wall &w,const Force &handle_force)
{
	for (unsigned int i = 0; i<balls.size(); i++)
       w.collidewithball(*balls[i],handle_force);
}
void Handle::draw(const Vector3 &color) 
{
    glPushMatrix();
    glColor3fv(color.toArray());
    Vector3 b=rotationaxis.getBegin();
    Vector3 e=rotationaxis.getEnd()-b;
	glTranslatef(b.X(), b.Y(), b.Z());

    glRotatef(thetarotate*180/PI,e.X(),e.Y(),e.Z());
	glTranslatef(-b.X(), -b.Y(), -b.Z());

	glColor3fv(color.toArray());
		for (unsigned int i = 0; i < players.size(); i++)
		{
			players[i]->draw(color);
		}
		for (unsigned int i = 0; i < balls.size(); i++)
		{
			balls[i]->draw(Vector3(0,0,0));
		}
		glPushMatrix();
		if (Htype==enemygoalkeeper||Htype==goalkeeper)
			glTranslated(centerofmass.X() /*+ Htype == enemygoalkeeper ?0* GOAL_WIDTH / 2 : 0*-GOAL_WIDTH / 2*/, centerofmass.Y(), centerofmass.Z());
		else if (Htype == enemydefenser || Htype == defenser)
			glTranslated(centerofmass.X() + Htype == enemydefenser ? GOAL_WIDTH / 2 : -GOAL_WIDTH / 2, centerofmass.Y(), centerofmass.Z());
		else if (Htype == enemyattacker || Htype == attacker)
			glTranslated(centerofmass.X() /*+ Htype == enemyattacker ? 0*GOAL_WIDTH / 2 :0* -GOAL_WIDTH / 2*/, centerofmass.Y(), centerofmass.Z());
		else if (Htype == enemymider || Htype == mider)
			glTranslated(centerofmass.X() + Htype == enemymider ? TABLE_WIDTH / 4 : -TABLE_WIDTH / 4, centerofmass.Y(), centerofmass.Z());

		glColor3f(0.5, 0.5, 0.5);
		if (Htype == enemyattacker || Htype == enemydefenser || Htype == enemygoalkeeper || Htype == enemymider)
			glRotated(-90, 0, 1, 0);
		else
            glRotated(90, 0, 1, 0);
		gluCylinder(gluNewQuadric(), radius, radius, ROD_LENGTH, radius * 50, radius * 50);
		glPopMatrix();

	   //non movement cylinder
		glPushMatrix();
		if (Htype == enemygoalkeeper || Htype == goalkeeper)
			glTranslated((StartPoint + Vector3(0, 0, (Htype == enemygoalkeeper ? -7.0f : 7.0f)*TABLE_DIPTH / 16.0f)).X() - TABLE_WIDTH / 2, (StartPoint + Vector3(0, 0, (Htype == enemygoalkeeper ? -7.0f : 7.0f)*TABLE_DIPTH / 16.0f)).Y(), (StartPoint + Vector3(0, 0,( Htype == enemygoalkeeper ? -7.0f : 7.0f)*TABLE_DIPTH / 16.0f)).Z());
		else if (Htype == enemydefenser || Htype == defenser)
			glTranslated((StartPoint + Vector3(0, 0, (Htype == enemydefenser ? -5.0f : 5.0f)*TABLE_DIPTH / 16.0f)).X() - TABLE_WIDTH / 2, (StartPoint + Vector3(0, 0, (Htype == enemydefenser ? -5.0f : 5.0f)*TABLE_DIPTH / 16.0f)).Y(), (StartPoint + Vector3(0, 0, (Htype == enemydefenser ? -5.0f : 5.0f)*TABLE_DIPTH / 16.0f)).Z());
		else if (Htype == enemyattacker || Htype == attacker)
			glTranslated((StartPoint + Vector3(0, 0, (Htype == enemyattacker ? 3.0f : -3.0f)*TABLE_DIPTH / 16.0f)).X() - TABLE_WIDTH / 2, (StartPoint + Vector3(0, 0, (Htype == enemyattacker ? 3.0f : -3.0f)*TABLE_DIPTH / 16.0f)).Y(), (StartPoint + Vector3(0, 0, (Htype == enemyattacker ? 3.0f : -3.0f)*TABLE_DIPTH / 16.0f)).Z());
		else if (Htype == enemymider || Htype == mider)
			glTranslated((StartPoint + Vector3(0, 0, (Htype == enemymider ? -1.0f : 1.0f)*TABLE_DIPTH / 16.0f)).X() - TABLE_WIDTH / 2, (StartPoint + Vector3(0, 0, (Htype == enemymider ? -1.0f : 1.0f)*TABLE_DIPTH / 16.0f)).Y(), (StartPoint + Vector3(0, 0, (Htype == enemymider ? -1.0f : 1.0f)*TABLE_DIPTH / 16.0f)).Z());

		glColor3f(0, 0, 0);
		glRotated(90, 0, 1, 0);
		gluCylinder(gluNewQuadric(), radius / 2, radius / 2, TABLE_WIDTH, radius * 50, radius * 50);
		glPopMatrix();
    glPopMatrix();
}


void Handle::applyforce(const Force &f, const bool &applyonorigin)
{
    Body::applyforce(f, applyonorigin);
	for (unsigned int i = 0; i<players.size(); i++)
        players[i]->applyforce(f,false);
	for (unsigned int i = 0; i<balls.size(); i++)
        balls[i]->applyforce(f,false);

}

void Handle::applytorque(const Force &f, const Vector3 &p, const bool &applyonorigin)
{
    Body::applytorque(f,p,applyonorigin);
    Vector3 torque=gettorquefromforce(f,p);
	for (unsigned int i = 0; i<players.size(); i++)
        players[i]->applytorque(torque,false);
	for (unsigned int i = 0; i<balls.size(); i++)
        balls[i]->applytorque(torque,false);
}

void Handle::applytorque(const Vector3 &torque, const bool &applyonorigin)
{
    Body::applytorque(torque,applyonorigin);
	for (unsigned int i = 0; i<players.size(); i++)
        players[i]->applytorque(torque,false);
	for (unsigned int i = 0; i<balls.size(); i++)
        balls[i]->applytorque(torque,false);
}
