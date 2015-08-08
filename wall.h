#ifndef WALL_H
#define WALL_H
#include "body.h"
#include "ball.h"
#include "plane.h"
#include <playsoundapi.h>

using namespace std;
class Quad
{
private :
    Vector3 p1,p2,p3,p4;
    Plane p;
    GLuint textid;
public :       
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
		glTexCoord2f(1, 1); glVertex3fv(p1.toArray());
		glTexCoord2f(0, 1); glVertex3fv(p2.toArray());
		glTexCoord2f(0, 0); glVertex3fv(p3.toArray());
		glTexCoord2f(1, 0); glVertex3fv(p4.toArray());
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
    void print()const
    {
        p1.print();
        cout<<'\n';
        p2.print();
        cout<<'\n';
        p3.print();
        cout<<'\n';
        p4.print();
    }
    void Translate(const Vector3 &v)
    {
        p1.Translate(v);
        p2.Translate(v);
        p3.Translate(v);
        p4.Translate(v);
        p.Translate(v);
    }
    void Rotate(const Vector3 &v,const float &theta)
    {
        p1.Rotate(v,theta);
        p2.Rotate(v,theta);
        p3.Rotate(v,theta);
        p4.Rotate(v,theta);
        p.Rotate(v,theta);
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
			
            if (!((theta<366.0&&theta>354.0)?360.0:roundf(theta)==360.0))
                return 0;
        }        
        return pint;
    }
};
class Wall : public Body
{    
private:
    const float dipth, width , height;
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
    void collidewithball(Ball &b,const Force &ballforce)
    {
       Vector3 *p;
       Quad q(*left);
       if((((p=up->collidewithball(b,q)))!=NULL)||((p=right->collidewithball(b,q))!=NULL)||((p=left->collidewithball(b,q))!=NULL)||((p=ground->collidewithball(b,q))!=NULL)||((p=front->collidewithball(b,q))!=NULL)||((p=back->collidewithball(b,q))!=NULL))
       {
		   if (origin){
			   cout << "hit with player"<<endl;
		   this->print();
		   }
		
		   float e=Body::RestCoffeciants.find(BodyPair(&b,this))->second;
           Vector3 v=(b.getvelocity()-velocity)*((e+1)/((1/b.getmass())+(1/mass)));
           Force J(v,v.length()/dt);
           Force friction(Vector3(),0);
           v=-b.getvelocity(*p)+getvelocity(*p);
		   v = Plane((b.getcenterofmass()-(*p)), *p).projectonplane(v);
           friction.setOrentation(-v);
           friction.setStrength(J.getStrength()*(*Body::StaticFrictionCoffeciants.find(BodyPair(&b,this))).second);
           if((transition.iszero())||(roundf(Vector3::abscosbetween(ballforce.getOrentation(),transition))!=1))
           {
               Plane qp=q.getplane();
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
               friction.setStrength(J.getStrength()*(*Body::StaticFrictionCoffeciants.find(BodyPair(&b,this))).second);
               applytorque(J+friction,*p,true);
               b.applyforce(-J,true);
           }
           if(!friction.getOrentation().iszero())
           {
               b.applytorque(-friction,*p,true);
           }
           else
           {
			   if (!b.getrotationalvelocity().iszero())
				   b.applytorque(-friction.getStrength()*(((*Ball::RollingFrictionCoffs.find(BodyPair(&b,this))).second)/(*Body::StaticFrictionCoffeciants.find(BodyPair(&b,this))).second),true);
           }
       }
	}
    void draw(const Vector3 &color) ;       
    bool operator ==(const Body *b) const;
    void print()const
    {
		cout << "Front(";
		front->print();
		cout <<")\n";
        front->print();
        cout<<"Back(";
        back->print();
        cout<<")\n";
		cout << "up(";

        up->print();
        cout<<")\n";
		cout << "Ground(";
        ground->print();
        cout<<")\n";
		cout << "right(";
        right->print();
        cout<<")\n";
		cout << "Left(";
        left->print();
		cout << ")\n";
    }
};

#endif // WALL_H
