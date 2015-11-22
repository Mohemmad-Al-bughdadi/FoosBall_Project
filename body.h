#ifndef BODY_H
#define BODY_H


#include "force.h"
#include "plane.h"
#include "line.h"
#include "Hash.h"
#include <map>

#define WALL_THICKNESS 4
#define TABLE_WIDTH 33
#define HALF_WIDTH	 TABLE_WIDTH/2
#define TABLE_DIPTH 59.25
#define HALF_DIPTH TABLE_DIPTH/2
#define TABLE_HEIGHT 10
#define HALF_HEIGHT TABLE_HEIGHT/2
#define LEG_HEIGHT 29
#define LEG_WIDTH 8
#define LEG_dipth 3.25
#define GOAL_HEIGHT TABLE_HEIGHT
#define GOAL_WIDTH 8.375
#define GOAL_dipth 3
#define BOARD_WIDTH (TABLE_WIDTH-GOAL_WIDTH)/2
const Vector3 StartPoint(0, 0, 0);
#define Player_WIDTH 1.5f
#define Player_HEIGHT (TABLE_HEIGHT/1.6)
#define Player_DIPTH 1.6f

#ifdef __linux__
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <tr1/unordered_map>
#include <SOIL/SOIL.h>
using namespace tr1;
#else

#include <gl\glut.h>
#include <gl\glext.h>
#endif


using namespace std;
#define dt 0.01
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
	
    static map<BodyPair,float> StaticFrictionCoffeciants;
	static map<BodyPair, float> RestCoffeciants;
    Body(const float &themass, Vector3 thecenter, const Line &r,const bool &fr,const Vector3 &frt,Body *o) :
		centerofmass(thecenter), mass(themass)
		, velocity(Vector3(0, 0, 0)), rotationaxis(r)
        , rotationalvelocity(0), thetarotate(0)
        ,freetochangerotationaxis(fr),transition(frt),origin(o)
	{

	}
    virtual ~Body(){}
    virtual void applyforce(const Force &f,const bool &applyonorigin)
    {
        float m=getoriginmass();							  
        if((origin!=0)&&(applyonorigin))        
            origin->applyforce(f,true);                                
        Vector3 oren=f.getOrentation();
        Vector3 force=Vector3::elementproduct(transition,oren)*f.getStrength();
        Vector3 accleration=force/m;
        velocity+=accleration*dt;

    }
    virtual void applytorque(const Force &f,const Vector3 &p,const bool &applyonorigin)
    {        
        if((origin!=0)&&(applyonorigin))
            origin->applytorque(f,p,true);
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
        if(((!freetochangerotationaxis)&&(Vector3::abscosbetween(torque,rotationaxis.getOrentiation())))||(freetochangerotationaxis))
        {
			Vector3 rotationalaccleration = torque / getorigininertia();
            rotationalvelocity+=rotationalaccleration*dt;
        }
    }

    virtual void draw(const Vector3 &color) =0;   
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
        centerofmass+=velocity*dt;
        thetarotate+=rotationalvelocity.length()*dt;
        while(thetarotate<0)
			thetarotate += TWOPI;
        while(thetarotate>TWOPI)
			thetarotate -= TWOPI;
    }
    Vector3 getvelocity(const Vector3 &p)const
    {
        if(!rotationaxis.notaline())
            return velocity+Plane(rotationaxis.getBegin(),rotationaxis.getEnd(),p).getNormal()*rotationalvelocity*rotationaxis.distancefrom(p);
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

