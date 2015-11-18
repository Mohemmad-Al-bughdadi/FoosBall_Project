#include "Handle.h"
#include "player.h"
#define SFC_HANDLE_WALL 0.2
#define RC_HANDLE_WALL 0.7
#define ROFC_HANDLE_WALL 0.4
#ifdef __linux__
void DrawCylinder(double radius,double height,Vector3 center,Vector3 axis)
{
    Vector3 pos=Vector3(0,0,height/2);
    double theta=Vector3::anglebetweeninradian(axis,Vector3(0,0,1));
    pos.Rotate(Vector3(0,0,0),Vector3(axis.Y(),axis.X(),0),theta);
    center-=pos;
    glPushMatrix();
    glTranslatef(center.X(),center.Y(),center.Z());
    glRotatef(theta/PIdiv180,axis.Y(),axis.X(),0);
    glutSolidCylinder(radius,height,50,50);
    glPopMatrix();
}
#else
void DrawCylinder(double radius,double height,Vector3 center,Vector3 axis)
{
    Vector3 pos=Vector3(0,0,height/2);
    double theta=Vector3::anglebetweeninradian(axis,Vector3(0,0,1));
    pos.Rotate(Vector3(axis.Y(),axis.X(),0),theta);
    center-=pos;
    glPushMatrix();
    glTranslatef(center.X(),center.Y(),center.Z());
    glRotatef(theta/PIdiv180,axis.Y(),axis.X(),0);
    gluCylinder(gluNewQuadric(), radius, radius, height, 50,50);
    glPopMatrix();
}
#endif



Handle::Handle(const Handle_Type &thetype, const double &handle_mass, const Vector3 &handle_center,const Line &handle_rotationaxis,const double &r,const bool &fr,const Vector3 &frt,Body *o)
:Body(handle_mass, handle_center, handle_rotationaxis,fr,frt,o), Htype(thetype), radius(r)
{    
    double ballsradii=radius+0.1;
    Vector3 axis=handle_rotationaxis.getOrentiation();
    axis.Normalize();
    if (thetype == goalkeeper)
    {
        balls.push_back(new Ball(ballsradii, handle_center + axis*TABLE_WIDTH/4, 0.01*handle_mass,handle_rotationaxis,false,frt, 0,this));
        players.push_back(new Player(handle_center, 0.6*handle_mass, handle_rotationaxis,false,frt, this));
        balls.push_back(new Ball(ballsradii, handle_center - axis*TABLE_WIDTH/4, 0.01*handle_mass,handle_rotationaxis,false,frt, 0,this));
    }

    else if (thetype == defenser)
    {
        balls.push_back(new Ball(ballsradii, handle_center + axis*(TABLE_WIDTH/6)+HALF_Player_WIDTH+ballsradii, 10,handle_rotationaxis,false,frt, 0,this));
        players.push_back(new Player(handle_center + axis*TABLE_WIDTH/6, 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center - axis*TABLE_WIDTH/6, 10, handle_rotationaxis,false,frt, this));
        balls.push_back(new Ball(ballsradii, handle_center - axis*(TABLE_WIDTH/6)-HALF_Player_WIDTH-ballsradii, 10,handle_rotationaxis,false,frt, 0,this));

	}	
    else if (thetype == attacker)
	{
        balls.push_back(new Ball(ballsradii, handle_center + axis*(TABLE_WIDTH/4)+HALF_Player_WIDTH+ballsradii, 10,handle_rotationaxis,false,frt, 0,this));
        players.push_back(new Player(handle_center + axis*TABLE_WIDTH/4, 10,handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center, 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center - axis*TABLE_WIDTH/4, 10, handle_rotationaxis,false,frt, this));
        balls.push_back(new Ball(ballsradii, handle_center - axis*(TABLE_WIDTH/4)-HALF_Player_WIDTH-ballsradii, 10,handle_rotationaxis,false,frt, 0,this));

    }   
    else if (thetype == mider)
	{
        balls.push_back(new Ball(ballsradii, handle_center + axis*(TABLE_WIDTH/3)+HALF_Player_WIDTH+ballsradii, 10,handle_rotationaxis,false,frt, 0,this));
        players.push_back(new Player(handle_center + axis*TABLE_WIDTH/6, 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center + axis*TABLE_WIDTH/3, 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center, 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center - axis*TABLE_WIDTH/3, 10, handle_rotationaxis,false,frt, this));
        players.push_back(new Player(handle_center - axis*TABLE_WIDTH/6, 10, handle_rotationaxis,false,frt, this));

        balls.push_back(new Ball(ballsradii, handle_center - axis*(TABLE_WIDTH/3)-HALF_Player_WIDTH-ballsradii, 10,handle_rotationaxis,false,frt, 0,this));

    }    
    double in=0.0f;
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
         Body::StaticFrictionCoffeciants.insert(pair<BodyPair,double>(BodyPair(balls[i],w),SFC_HANDLE_WALL));
         Body::RestCoffeciants.insert(pair<BodyPair,double>(BodyPair(balls[i],w),RC_HANDLE_WALL));
         Ball::RollingFrictionCoffs.insert(pair<BodyPair, double>(BodyPair(balls[i], w), ROFC_HANDLE_WALL));
    }
}

void Handle::collidewallwithballs(Wall &w,const Force &handle_force)
{
    for (unsigned int i = 0; i<balls.size(); i++)
        w.collidewithball(*balls[i],handle_force);
}

void Handle::draw(const Vector3 &color) const
{
    for (unsigned int i = 0; i < players.size(); i++)
    {
        players[i]->draw(color);
    }
    for (unsigned int i = 0; i < balls.size(); i++)
    {
        balls[i]->draw(Vector3(0,0,0));
    }        

    glColor3dv(color.toArray());

    DrawCylinder(radius,ROD_LENGTH,centerofmass,rotationaxis.getOrentiation());
    DrawCylinder(radius/2,ROD_LENGTH,centerofmass,rotationaxis.getOrentiation());
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
