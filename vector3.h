#ifndef VECTOR3_H
#define VECTOR3_H
#include <cmath>
#include <iostream>
#include <exception>

#define PI 3.141592653589f
#define TWOPI  2*PI
#define gravity 9.7973f
#define PIdiv180 (PI/180.0f)

using namespace std;

class Vector3
{
private:
    float x,y,z;
public:   
    Vector3(float coordx=0,float coordy=0,float coordz=0):x(coordx),y(coordy),z(coordz)
    {  
    }
    bool iszero()const
    {
        return((x>=0.0001f?x:roundf(x))==0)&&((y>=0.0001?y:roundf(y))==0)&&((z>=0.0001?z:roundf(z))==0);
    }
    Vector3(const Vector3 &v):x(v.x),y(v.y),z(v.z)
    {
    }
    void print()const
    {
        cout<<'( '<<x<<' , '<<y<<' , '<<z<<' ) \n';
    }
    float length()const
    {
        return sqrt(x*x+y*y+z*z);
    }
    float X()const
    {
        return x;
    }
    float Y()const
    {
        return y;
    }
    float Z()const
    {
        return z;
    }
    static float anglebetweeninradian(const Vector3 &v1,const Vector3 &v2)
    {		
        return acos((v1*v2)/(v1.length()*v2.length()));
    }
    void Normalize()
    {
        float len=length();
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
    Vector3 operator *(const float &scale)const
    {
        return Vector3(x*scale,y*scale,z*scale);
    }
    static Vector3 elementproduct(const Vector3 &v,const Vector3 &u)
    {
        return Vector3(v.x*u.x,v.y*u.y,v.z*u.z);
    }

    float operator *(const Vector3 &v)const
    {
        return x*v.x+y*v.y+z*v.z;
    }
    Vector3 operator /(const float &scale)const
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
    Vector3 &operator *=(const float& scale)
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
    Vector3 &operator /=(const float& scale)
    {
        try
        {
            x/=scale;
            y/=scale;
            z/=scale;
            return *this;
        }
        catch(exception e)
        {
            cerr<<e.what()<<'\n';
        }
    }
    static float abscosbetween(const Vector3 &v1,const Vector3 &v2)
    {													
		return fabs((v1*v2) / (v1.length()*v2.length()));
    }

    void Translate(const Vector3 &v)
    {
        x+=v.x;
        y+=v.y;
        z+=v.z;
    }
    void RotateX(const float &angle)
    {
        float y0=y;
        float z0=z;
        float theta=(float)(angle*PIdiv180);
        float cost=(float)cos(theta);
        float sint=(float)sin(theta);

        z=y0*sint+z0*cost;
        y=y0*cost-z0*sint;
    }
    void RotateY(const float &angle)
    {
        float x0=x;
        float z0=z;
        float theta=(float)(angle*PIdiv180);
        float cost=(float)cos(theta);
        float sint=(float)sin(theta);

        z=x0*sint+z0*cost;
        x=x0*cost-z0*sint;

    }
    void RotateZ(const float &angle)
    {
        float x0=x;
        float y0=y;
        float theta=(float)(angle*PIdiv180);
        float cost=(float)cos(theta);
        float sint=(float)sin(theta);

        y=x0*sint+y0*cost;
        x=x0*cost-y0*sint;
    }

    Vector3 Rotate(Vector3 v,const float &theta1)  const
    {
		float a = 0, b = 0, c = 0;

        float theta=(float)(theta1*PIdiv180);

        a=(cos(theta)+(v.x*v.x)*(1-cos(theta)))*x;
        a+=(v.x*v.y*(1-cos(theta))-v.z*sin(theta))*y;
        a+=(v.x*v.z*(1-cos(theta))+v.y*sin(theta))*z;

        b=(v.y*v.x*(1-cos(theta))+v.z*sin(theta))*x;
        b+=(cos(theta)+v.y*v.y*(1-cos(theta)))*y;
        b+=(v.y*v.z*(1-cos(theta))-v.x*sin(theta))*z;

        c=(v.z*v.x*(1-cos(theta))-v.y*sin(theta))*x;
        c+=(v.z*v.y*(1-cos(theta))+v.x*sin(theta))*y;
        c+=(cos(theta)+v.z*v.z*(1-cos(theta)))*z;
		return Vector3(a, b, c);
    }
    void Scale(const float &c)
    {
        x*=c;
        y*=c;
        z*=c;
    }

     float *toArray()  const
    {
        float *a=new float[3];
        a[0]=x;
        a[1]=y;
        a[2]=z;
        return a;
    }
    static Vector3 crossproduct(const Vector3 &v,const Vector3 &u)
    {
        Vector3 resVector;
        resVector.x = u.y*v.z - u.z*v.y;
        resVector.y = u.x*v.z - u.z*v.x;
        resVector.z = u.x*v.y - u.y*v.x;

        return resVector;
    }
};
#endif // VECTOR3_H
