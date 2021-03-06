#ifndef WALL_H
#define WALL_H
#include "body.h"
#include "ball.h"
#include "plane.h"


using namespace std;
class Quad
{
private :
    Vector3 p1,p2,p3,p4;
    Plane p;
    GLuint textid;
public :       
    void checknormal(const Vector3 &point)
    {
        if(p.ispointupper(point))
            p.reverse();
    }

    Quad(const Vector3 &v1,const Vector3 &v2,const Vector3 &v3,const Vector3 &v4,const GLuint &texture)
		:p1(v1), p2(v2), p3(v3), p4(v4), p(Plane(p1, p2, p3)), textid(texture)
    {

    }
    ~Quad(){}
    void draw(Vector3 color)
    {
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textid);
		glEnable(GL_TEXTURE_2D);
        glColor3fv(color.toArray());
        glBegin(GL_QUADS);
        glNormal3fv(p.getNormal().toArray());
        glTexCoord2d(1, 1); glVertex3fv(p1.toArray());
        glTexCoord2d(0, 1); glVertex3fv(p2.toArray());
        glTexCoord2d(0, 0); glVertex3fv(p3.toArray());
        glTexCoord2d(1, 0); glVertex3fv(p4.toArray());
        glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
    }   
    Quad &operator =(const Quad &q)
    {
        p1=q.p1;
        p2=q.p2;
        p3=q.p3;
        p4=q.p4;
        p=q.p;
        textid=q.textid;
        return *this;
    }   
    void Translate(const Vector3 &v)
    {
        p1.Translate(v);
        p2.Translate(v);
        p3.Translate(v);
        p4.Translate(v);
        p.Translate(v);
    }
    void Rotate(const Vector3 &p11,const Vector3 &p21,const float &theta)
    {
        p1.Rotate(p11,p21,theta);
        p2.Rotate(p11,p21,theta);
        p3.Rotate(p11,p21,theta);
        p4.Rotate(p11,p21,theta);
        p.Rotate(p11,p21,theta);
    }
    Plane getplane()const
    {
        return p;
    }
    Vector3 *collidewithball(Ball &b,Quad &q)
    {		
        Vector3 *pint=b.collidewithball(p);
        q=*this;       
        if(pint!=NULL)
        {           
            Vector3 pintde=*pint;
            float theta= Vector3::anglebetweeninradian(p1-pintde,p2-pintde)
                    +Vector3::anglebetweeninradian(p2-pintde,p3-pintde)
                    +Vector3::anglebetweeninradian(p3-pintde,p4-pintde)
                    +Vector3::anglebetweeninradian(p4-pintde,p1-pintde);
            theta*=180/PI;            
            if(!floatsequal(theta,360))
                return 0;
        }        
        return pint;
    }
};
class Wall : public Body
{    
private:
    const float dipth, width , height;
    void checknormals();
public:
	Quad *left, *right, *front, *back, *ground, *up;

    Wall(const float &thewidth,const float &theheight,const float &thedipth, const float &mass,Vector3 center,const Line &rot,const bool &fr,const Vector3 &frt,Body *o,GLuint*tids);
    ~Wall()
    {
        delete left;
        delete right;
        delete front;
        delete back;
        delete ground;
        delete up;
    }
    float getdipth()const
    {
        return dipth;
    }
    float getheight()const
    {
        return height;
    }
    float getwidth()const
    {
        return width;
    }
    void collidewithball(Ball &b,Force &ballforce)
    {
       Vector3 *p;
       Quad q(*left);
       if((((p=up->collidewithball(b,q)))!=NULL)||((p=right->collidewithball(b,q))!=NULL)||((p=left->collidewithball(b,q))!=NULL)||((p=ground->collidewithball(b,q))!=NULL)||((p=front->collidewithball(b,q))!=NULL)||((p=back->collidewithball(b,q))!=NULL))
       {		 
           float e=Body::RestCoffeciants.find(BodyPair(&b,this))->second;
           Force J;
		   Vector3 totalvelocity(velocity + getvelocity(*p));
		   Vector3 v = b.getvelocity() - totalvelocity;
         /*  if(!v.iszero())
           {
              Plane plane=q.getplane();
              v=v-plane.projectonplane(v);
              v=v*((e+1)/((1/b.getmass())+(1/mass)));
              if(Vector3::anglebetweeninradian(v,plane.getNormal())!=0)
              {
                  J.setOrentation(v);
                  J.setStrength(v.length()/dt);
              }
           }*/
		   J.setOrentation(v);
		   J.setStrength((v*((e + 1) / ((1 / b.getmass()) + (1 / mass))) / dt).length());

           Force friction(Vector3(),0);
           Force Normal;
           v=b.getvelocity(*p)-getvelocity(*p);
           Vector3 v1=v;
           if(!v.iszero())
           {
               v = Plane((b.getcenterofmass()-(*p)), *p).projectonplane(v);
               friction.setOrentation(-v);
           }
           if((transition.iszero())||(Vector3::abscosbetween(ballforce.getOrentation(),transition)!=1))
           {
               Line line(*p,(*p)+ballforce.getOrentation());
               if(Line::isparallel(line,rotationaxis)||Line::intersect(line,rotationaxis))
               {
                   Plane qp=q.getplane();
                   Vector3 nor=qp.getNormal();
                   Vector3 ballorigin=b.getcenterofmass();
                   if(((qp.isnormaldown())&&(qp.ispointupper(ballorigin)))||((!qp.isnormaldown())&&(qp.ispointlower(ballorigin))))
                       nor=-nor;
                   float theta=Vector3::cosbetween(ballforce.getOrentation(),nor);
                   if(theta<0)
                       Normal=Force(nor,ballforce.getStrength()*fabs(theta));
                   Force total(Normal-J);
                   friction.setStrength(total.getStrength()*(*Body::StaticFrictionCoffeciants.find(BodyPair(&b,this))).second);
                   ballforce+=total;
               }
               else
               {
                   friction.setStrength(J.getStrength()*(*Body::StaticFrictionCoffeciants.find(BodyPair(&b,this))).second);
                   applytorque(J+friction,*p,true);
                   ballforce-=J;
               }
           }
		   else 
           {
               friction.setStrength(J.getStrength()*(*Body::StaticFrictionCoffeciants.find(BodyPair(&b,this))).second);
               applytorque(J+friction,*p,true);
               ballforce-=J;
           }
           if(!v.iszero())
           {
               friction.setStrength(friction.getStrength()*(*Ball::RollingFrictionCoffs.find(BodyPair(&b,this))).second/(*Body::StaticFrictionCoffeciants.find(BodyPair(&b,this))).second);
               b.applytorque(-friction.getStrength(),true);
               ballforce+=friction;
           }
           else if((v1.iszero())&&(!ballforce.iszero()))
           {
               Force ballforceprojection(Plane((b.getcenterofmass()-(*p)),*p).projectonplane(ballforce.getOrentation()*ballforce.getStrength()));               
			   //��� ���� ��� ����� �������� ���� �������� �� ���� ��� ����� �������� �� �����
			   ballforceprojection = ballforce - ballforceprojection;
               friction.setOrentation(-ballforceprojection.getOrentation());               
               friction.setStrength(Normal.getStrength()*(*Body::StaticFrictionCoffeciants.find(BodyPair(&b,this))).second);
               Force tot=ballforceprojection+friction;
               if(floatsequal(Vector3::anglebetweeninradian(tot.getOrentation(),friction.getOrentation()),0))
                   ballforce=Force();
               else
                   b.applytorque(friction,*p,true);
           }
       }
	}
    void draw(const Vector3 &color)const;
    void proceedintime();
    void Translate(const Vector3 &diff);
    void Rotate(const float &diff);
    bool operator ==(const Body *b) const;    
};

#endif // WALL_H
