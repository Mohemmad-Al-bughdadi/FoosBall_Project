#ifndef BODY_H
#define BODY_H


#include "force.h"
#include "plane.h"
#include "line.h"
#include "Hash.h"
#include <map>

#define WALL_THICKNESS 3
#define HALF_WALL_THICKNESS WALL_THICKNESS/2
#define TABLE_WIDTH 36
#define HALF_WIDTH	 TABLE_WIDTH/2
#define TABLE_DIPTH 64
#define HALF_DIPTH TABLE_DIPTH/2
#define TABLE_HEIGHT 10
#define HALF_HEIGHT TABLE_HEIGHT/2
#define LEG_HEIGHT 29
#define LEG_WIDTH 8
#define LEG_DIPTH 3.25
#define GOAL_HEIGHT 8
#define GOAL_WIDTH 8.375
#define GOAL_dipth 3
#define BOARD_WIDTH (TABLE_WIDTH-GOAL_WIDTH)/2
#define HALF_GOAL_HEIGHT GOAL_HEIGHT/2
#define HALF_LEG_HEIGHT LEG_HEIGHT/2
#define HALF_LEG_DIPTH LEG_DIPTH/2
#define HALF_LEG_WIDTH LEG_WIDTH/2
const Vector3 StartPoint(0, 0, 0);
#define Player_WIDTH 1.3f
#define Player_HEIGHT (TABLE_HEIGHT-1)
#define Player_DIPTH 1.3f
#define HALF_Player_WIDTH Player_WIDTH/2

#ifdef __linux__
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <tr1/unordered_map>
using namespace tr1;
#else
#include <gl\glut.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <unordered_map>
#endif


using namespace std;
#define dt 0.02
class Body
{
protected:   
	Vector3 centerofmass;
	const float mass;
    float inertia;
	Vector3 velocity;   
	Line rotationaxis;
    Vector3 rotationalvelocity;
	float thetarotate;
    bool freetochangerotationaxis;
    Vector3 transition;
    Body *origin;
    Vector3 gettorquefromforce(const Force &f,const Vector3 &p)const
    {
        Vector3 beg=rotationaxis.getBegin(),end=rotationaxis.getEnd();
        Plane p1(p,beg,end);
        Plane c(beg,end,beg+p1.getNormal());
        Vector3 r=-(c.getNormal()*rotationaxis.distancefrom(p));
        if(!c.ispointin(p+r))
            r=-r;
        return Vector3::crossproduct(r,f.getOrentation()*f.getStrength());
    }
    float getoriginmass()const
    {
        if(origin!=0)
            return origin->getoriginmass();
        return mass;
    }
    float getorigininertia()const
    {
        if(origin!=0)
            return origin->getorigininertia();
        return getinertia();
    }
public:
	
    static unordered_map<BodyPair,float,KeyHasher> StaticFrictionCoffeciants;
    static unordered_map<BodyPair, float,KeyHasher> RestCoffeciants;
    Body(const float &themass, Vector3 thecenter, const Line &r,const bool &fr,const Vector3 &frt,Body *o) :
		centerofmass(thecenter), mass(themass)
		, velocity(Vector3(0, 0, 0)), rotationaxis(r)
        , rotationalvelocity(0), thetarotate(0)
        ,freetochangerotationaxis(fr),transition(frt),origin(o)
	{

	}
    virtual void stopRotation(const bool &applyonorigin)
    {
        if((origin!=NULL)&&(applyonorigin))
            origin->stopRotation(true);
        else
        {
            rotationalvelocity.reset();
        }
    }
    virtual void stopMovement(const bool &applyonorigin)
    {
        if((origin!=NULL)&&(applyonorigin))
            origin->stopMovement(true);
        else
        {
            velocity.reset();
        }
    }
    void stop(const bool &applyonorigin)
    {
        stopRotation(applyonorigin);
        stopMovement(applyonorigin);
    }

    virtual ~Body(){}
    virtual void applyforce(const Force &f,const bool &applyonorigin)
    {
        float m=getoriginmass();							  
        if((origin!=0)&&(applyonorigin))
            origin->applyforce(f,true);
        else
        {
            if(!f.iszero())
            {
                Vector3 oren=f.getOrentation();
                Vector3 force=Vector3::elementproduct(transition,oren)*f.getStrength();
                Vector3 accleration=force/m;
                velocity+=accleration*dt;                    
            }
        }

    }
    virtual void applytorque(const Force &f,const Vector3 &p,const bool &applyonorigin)
    {               
        Vector3 tor=gettorquefromforce(f,p);
        if(!tor.iszero())
            applytorque(tor,applyonorigin);
        else
            applyforce(f,applyonorigin);
    }

    virtual void applytorque(const Vector3 &torque,const bool &applyonorigin)
    {
        if((origin!=0)&&(applyonorigin))
            origin->applytorque(torque,true);
        else
        {
            if(((!freetochangerotationaxis)&&(Vector3::abscosbetween(torque,rotationaxis.getOrentiation())==1))||(freetochangerotationaxis))
            {
                Vector3 rotationalaccleration = torque / getorigininertia();
                rotationalvelocity+=rotationalaccleration*dt;
                Vector3 halfrot=(rotationaxis.getBegin()+rotationaxis.getEnd())/2;
                rotationaxis=Line(halfrot,halfrot+rotationalvelocity);
            }
        }
    }

    virtual void draw(const Vector3 &color)const=0;
    virtual bool operator ==(const Body *b)const
    {
        return (mass==b->mass)&&(inertia==b->inertia);
    }
    Vector3 getrotationalvelocity()const
    {
        return rotationalvelocity;
    }
    float whathasbeenrotated()const
    {
        return thetarotate;
    } 
    float getmass() const
    {
        return mass;
    }
    float getinertia()const
    {
        float r=rotationaxis.distancefrom(centerofmass);
        return inertia+mass*r*r;
    }
    virtual void proceedintime()
    {        
        Vector3 deltan(velocity*dt);
        centerofmass+=deltan;
        thetarotate+=rotationalvelocity.length()*dt;
    /*    while(thetarotate<0)
			thetarotate += TWOPI;
        while(thetarotate>TWOPI)
            thetarotate -= TWOPI;*/
        if(freetochangerotationaxis)
            rotationaxis=Line(rotationaxis.getBegin()+deltan,rotationaxis.getEnd()+deltan);
    }
    Vector3 getvelocity(const Vector3 &p)const
    {
        if(!rotationaxis.notaline())
            return velocity+Plane(rotationaxis.getBegin(),rotationaxis.getEnd(),p).getNormal()*rotationalvelocity.length()*rotationaxis.distancefrom(p);
        return velocity;
    }
    Vector3 getvelocity()const
    {
        return velocity;
    }   
    Vector3 getcenterofmass()const
    {
        return centerofmass;
    }       
};
#endif // BODY_H
