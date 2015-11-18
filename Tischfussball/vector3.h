#ifndef VECTOR3_H
#define VECTOR3_H
#include <cmath>
#include <iostream>
#include <exception>
#include <doublehelper.h>
#define PI 3.141592653589f
#define TWOPI  2*PI
#define gravity 9.7973f
#define PIdiv180 (PI/180.0f)

using namespace std;
class Vector3
{
private:
    double x,y,z;
    double sqr(const double &x)const
    {
        return x*x;
    }

public:   
    Vector3(double coordx=0,double coordy=0,double coordz=0):x(coordx),y(coordy),z(coordz)
    {  
    }
    bool iszero()const
    {
        return (doublesequal(x,0))&&(doublesequal(y,0))&&(doublesequal(z,0));
    }
    Vector3(const Vector3 &v):x(v.x),y(v.y),z(v.z)
    {
    }    
    double length()const
    {
        return sqrt(x*x+y*y+z*z);
    }
    double X()const
    {
        return x;
    }
    double Y()const
    {
        return y;
    }
    double Z()const
    {
        return z;
    }
    static double anglebetweeninradian(const Vector3 &v1,const Vector3 &v2)
    {		
        return acos((v1*v2)/(v1.length()*v2.length()));
    }
    void Normalize()
    {
        double len=length();
        if(len==0)
            return;
        if(len!=1)
        {
            x/=len;
            y/=len;
            z/=len;
        }
    }
    bool operator ==(const Vector3 &v)const
    {
        return (x==v.x)&&(y==v.y)&&(z==v.z);
    }
    bool operator !=(const Vector3 &v)const
    {
        return !operator==(v);
    }
    Vector3 operator +(const Vector3 &other)const
    {
        return Vector3(x+other.x,y+other.y,z+other.z);
    }
    Vector3 operator -(const Vector3 &other)const
    {
        return Vector3(x-other.x,y-other.y,z-other.z);
    }
    Vector3 operator -()const
    {
        return Vector3(-x,-y,-z);
    }
    Vector3 operator *(const double &scale)const
    {
        return Vector3(x*scale,y*scale,z*scale);
    }
    static Vector3 elementproduct(const Vector3 &v,const Vector3 &u)
    {
        return Vector3(v.x*u.x,v.y*u.y,v.z*u.z);
    }

    double operator *(const Vector3 &v)const
    {
        return x*v.x+y*v.y+z*v.z;
    }
    Vector3 operator /(const double &scale)const
    {
        try
        {
            return Vector3(x/scale,y/scale,z/scale);
        }
        catch(exception e)
        {
            cerr<<e.what()<<'\n';
            return 0;

        }
    }
    Vector3 &operator =(const Vector3& other)
    {
        x=other.x;
        y=other.y;
        z=other.z;
        return *this;
    }

    Vector3 &operator +=(const Vector3& other)
    {
        x+=other.x;
        y+=other.y;
        z+=other.z;
        return *this;
    }
    Vector3 &operator -=(const Vector3& other)
    {
        x-=other.x;
        y-=other.y;
        z-=other.z;
        return *this;
    }
    Vector3 &operator *=(const double& scale)
    {
        x*=scale;
        y*=scale;
        z*=scale;
        return *this;
    }
    static bool isperpendicular(const Vector3 &u,const Vector3 &v)
    {
        return (u*v) == 0;
    }
    static bool isparallel(const Vector3 &u,const Vector3 &v)
    {
        Vector3 zero;
        if((v==zero)||(u==zero))
            return true;
        return ((u.x/v.x)==(u.y/v.y))&&((u.y/v.y)==(u.z/v.z));
    }
    Vector3 &operator /=(const double& scale)
    {
        x/=scale;
        y/=scale;
        z/=scale;
        return *this;
    }
    static double abscosbetween(const Vector3 &v1,const Vector3 &v2)
    {													
		return fabs((v1*v2) / (v1.length()*v2.length()));
    }

    void Translate(const Vector3 &v)
    {
        x+=v.x;
        y+=v.y;
        z+=v.z;
    }
    void RotateX(const double &angle)
    {
        double y0=y;
        double z0=z;
        double theta=(double)(angle*PIdiv180);
        double cost=(double)cos(theta);
        double sint=(double)sin(theta);

        z=y0*sint+z0*cost;
        y=y0*cost-z0*sint;
    }
    void RotateY(const double &angle)
    {
        double x0=x;
        double z0=z;
        double theta=(double)(angle*PIdiv180);
        double cost=(double)cos(theta);
        double sint=(double)sin(theta);

        z=x0*sint+z0*cost;
        x=x0*cost-z0*sint;

    }
    void RotateZ(const double &angle)
    {
        double x0=x;
        double y0=y;
        double theta=(double)(angle*PIdiv180);
        double cost=(double)cos(theta);
        double sint=(double)sin(theta);

        y=x0*sint+y0*cost;
        x=x0*cost-y0*sint;
    }

    void Rotate(const Vector3 &p1,const Vector3 &p2,const double &theta)
    {
        Vector3 orentation=p2-p1;
        orentation.Normalize();
        double u=orentation.X(),v=orentation.Y(),w=orentation.Z(),a=p1.X(),b=p1.Y(),c=p1.Z();
        double resX=((a*(sqr(v) + sqr(w))) - u*(b*v + c*w - u*x - v*y - w*z))*(1 - cos(theta)) + x*cos(theta) + (-c*v + b*w - w*y + v*z)*sin(theta);
        double resY=((b*(sqr(u)+sqr(w))) - v*(a*u + c*w - u*x - v*y - w*z))*(1 - cos(theta)) + y* cos(theta) + (c*u - a*w + w*x - u*z)*sin(theta);
        double resZ=((c*(sqr(u)+sqr(v))) - w*(a*u + b*v - u*x - v*y - w*z))*(1 - cos(theta)) + z* cos(theta) + (-b*u + a*v - v*x + u*y)*sin(theta);
        x=resX;
        y=resY;
        z=resZ;
    }
    void RotateAsVector(const Vector3 &p1,const Vector3 &p2,const double &theta)
    {
        Vector3 zero,res(*this);
        zero.Rotate(p1,p2,theta);
        res.Rotate(p1,p2,theta);
        res-=zero;
        x=res.x;
        y=res.y;
        z=res.z;
    }

    void Scale(const double &c)
    {
        x*=c;
        y*=c;
        z*=c;
    }

    double *toArray()  const
    {
        double *a=new double[3];
        a[0]=x;
        a[1]=y;
        a[2]=z;
        return a;
    }
    static Vector3 crossproduct(const Vector3 &u,const Vector3 &v)
    {
        Vector3 resVector;
        resVector.x = u.y*v.z - u.z*v.y;
        resVector.y = u.x*v.z - u.z*v.x;
        resVector.z = u.x*v.y - u.y*v.x;

        return resVector;
    }
};


#endif // VECTOR3_H
