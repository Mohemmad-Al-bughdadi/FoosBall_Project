#ifndef HANDLE_H
#define HANDLE_H
#define STICK 35
#define ROD_LENGTH TABLE_WIDTH+STICK
#include "ball.h"
#include "player.h"
#include <vector>
enum Handle_Type
{
	goalkeeper, //1 fossman
	defenser, //2 fossman	
	mider, //5fossman	
	attacker, //3fossman	
};
class Handle :public Body
{
private:
	Handle_Type Htype;
	const float radius;
    vector<Player*> players;
    vector<Ball*> balls;
    public:
        Handle(const Handle_Type &thetype, const float &handle_mass, const Vector3 &handle_center,const Line &handle_rotationaxis,const float &r,const bool &fr,const Vector3 &frt,Body *o);
        ~Handle()
        {
			for (unsigned int i = 0; i<players.size(); i++)
                delete players[i];
			for (unsigned int i = 0; i<balls.size(); i++)
                delete balls[i];
        }

       virtual void draw(const Vector3 &color)const;
	   bool operator ==(const Body *b) const;
       void stopRotation(const bool &applyonorigin);
       void stopMovement(const bool &applyonorigin);
       int numberofplayers()const
       {
           return players.size();
       }
       int numberofballs()const
       {
           return balls.size();
       }
       Ball *getball(const int &i)const
       {
           return balls[i];
       }
       Player *getplayer(const int &i)const
       {
           return  players[i];
       }
       void proceedintime();
       void collidewithball(Ball &b,Force &ballforce)
       {
           Vector3 cen=b.getcenterofmass();
           float brad=b.getRadius();
           /*if(rotationaxis.distancefrom(cen)<=radius+brad)
           {
               Vector3 beg=rotationaxis.getBegin(),end=rotationaxis.getEnd();
               Vector3 nor=Plane(cen,beg,end).getNormal();
               nor=Vector3::crossproduct(nor,rotationaxis.getOrentiation());
               if(rotationaxis.distancefrom(beg)>radius)
                   beg=cen-(nor*brad);
               else
                   beg=cen+(nor*brad);

               float e=(*Body::RestCoffeciants.find(BodyPair(&b,this))).second;
               Vector3 v=(b.getvelocity()-velocity)*((e+1)/((1/b.getmass())+(1/mass)));
               Force J(v,v.length()/dt);
               Force friction(Vector3(),0);
               v=-b.getvelocity(beg)+getvelocity(beg);
			   Plane qp(rotationaxis.projectonline(beg)-beg,beg);             
			   friction.setOrentation(qp.projectonplane(v));
               nor=cen-beg;			  
               Force Normal(nor,ballforce.getStrength()*Vector3::abscosbetween(ballforce.getOrentation(),nor));
               Force total(Normal-J);
               friction.setStrength(total.getStrength()*(*Body::StaticFrictionCoffeciants.find(BodyPair(&b,this))).second);
               b.applyforce(total,false);
               if(friction.getOrentation()!=Vector3())
               {
                   b.applytorque(-friction,beg,false);
               }
               else
               {
                   b.applytorque(-friction.getStrength()*(((*Ball::RollingFrictionCoffs.find(BodyPair(this,&b))).second)/(*Body::StaticFrictionCoffeciants.find(BodyPair(&b,this))).second),false);
               }
               applytorque(friction,beg,true);
           }*/
		   for (unsigned int i = 0; i<players.size(); i++)
               players[i]->collidewithball(b,ballforce);
       }
       void fillconstants(Wall *w)const;
       void collidewallwithballs(Wall &w,Force &handle_force);
       void applyforce(const Force &f, const bool &applyonorigin);
       void applytorque(const Force &f, const Vector3 &p, const bool &applyonorigin);
       void applytorque(const Vector3 &torque,const bool &applyonorigin);
};

#endif
