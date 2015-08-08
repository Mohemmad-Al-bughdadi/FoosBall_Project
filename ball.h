#ifndef BALL_H
#define BALL_H
#include "body.h"
#include "line.h"
#include "plane.h"


class Ball : public Body
{
private:
    float radius;    
    const unsigned int texture_id;
public:
    static map<BodyPair,float> RollingFrictionCoffs;
    Ball(const float &theradius,const Vector3 &origin,const float &mass,const Line &rot,const bool &fr,const Vector3 &tr,const GLuint &theTexure,Body *o);
    ~Ball(){}
	virtual void draw(const Vector3 &color);
    float getRadius() const;
	Vector3 *collidewithball(const Line &l)const
	{
		Vector3 *p = 0;
		if (l.distancefrom(centerofmass) <= radius)
		{
			Vector3 beg = l.getBegin();
			Vector3 oren = l.getOrentiation();
			float L = (beg - centerofmass).length();
			float t = sqrt(L*L - radius*radius);
			p = new Vector3(beg + oren*t);
			if (((*p) - centerofmass).length() > L)
			{
				delete p;
				p = new Vector3(beg - oren*t);
			}
		}
		return p;
	}
	void print()
	{
		cout << "Ball(" << this->centerofmass.X() << "," << this->centerofmass.Y() << "," << this->centerofmass.Z() << ")\n";
	}
    Vector3 *collidewithball(const Plane &p)const
    {
        Vector3 ballorigin=centerofmass;
        if(p.distancefrom(ballorigin)<=radius)
        {           
			ballorigin.Translate(p.getNormal()*(radius));
            if(p.ispointin(ballorigin))
				ballorigin.Translate((-p.getNormal())*radius*2 );
            return new Vector3(ballorigin);
        }
        return 0;
    }
    Vector3 *ballwithball(const Ball &b)const
    {
        Vector3 ballorigin=b.centerofmass;
        if((ballorigin-centerofmass).length()<=radius+b.radius)
        {
            Line l(ballorigin,centerofmass);
            Vector3 v=l.getOrentiation();
            if((ballorigin+v)!=(centerofmass))
                v=-v;
            v.Normalize();
            return new Vector3(ballorigin+(v*b.radius));
        }
    }
    void collideballwithball(Ball &b,const Force &ballforce)
    {
        Vector3 *p=0;
        if(p=ballwithball(b))
        {
            float e=Body::RestCoffeciants.find(BodyPair(&b,this))->second;
            Vector3 v=(b.getvelocity()-velocity)*((e+1)/((1/b.getmass())+(1/mass)));
            Force J(v,v.length()/dt);
            Force friction(Vector3(),0);
            v=-b.getvelocity(*p)+getvelocity(*p);
            v=Plane(((*p)-b.centerofmass),*p).projectonplane(v);
            friction.setOrentation(-v);
            friction.setStrength(J.getStrength()*(*Body::StaticFrictionCoffeciants.find(BodyPair(&b,this))).second);
            if((transition.iszero())||(Vector3::abscosbetween(ballforce.getOrentation(),transition)!=1))
            {
				Plane qp(b.getcenterofmass()-(*p),*p);
                Vector3 nor=qp.getNormal();
                Vector3 ballorigin=b.getcenterofmass();
                if(((qp.isnormaldown())&&(qp.ispointupper(ballorigin)))||((!qp.isnormaldown())&&(qp.ispointlower(ballorigin))))
                        nor=-nor;
                Force Normal(nor,ballforce.getStrength()*Vector3::abscosbetween(ballforce.getOrentation(),nor));
                Force total(Normal-J);
                friction.setStrength(total.getStrength()*(*Body::StaticFrictionCoffeciants.find(BodyPair(&b,this))).second);
                b.applyforce(total,true);
            }
            else
            {
                friction.setStrength(J.getStrength()*0.4);
                applytorque(J/*+friction*/,*p,true);
                b.applyforce(-J,true);
            }
            /*if(friction.getStrength()!=0)
            {
                b.applytorque(-friction,*p,true);
            }
            else
            {
                b.applytorque(-friction.getStrength()*(*Ball::RollingFrictionCoffs.find(BodyPair(this,&b))).second,true);
            }*/
        }
    }

    bool operator ==(const Body *b) const;   
};

#endif // BALL_H
