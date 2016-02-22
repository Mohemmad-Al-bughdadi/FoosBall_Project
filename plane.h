#ifndef PLANE_H
#define PLANE_H
#include "vector3.h"

struct Plane
{
private:
    Vector3 normal;
    Vector3 referencepoint;    
    float d;
public:
    Plane(const Vector3 &n,const Vector3 &r)
        :normal(n),referencepoint(r)
    {
        normal.Normalize();
        d=-(normal*referencepoint);
    }
    Plane(const Vector3 &p1,const Vector3 &p2,const Vector3 &p3)
        :normal(Vector3::crossproduct(p1-p2,p3-p2)),referencepoint(p2)
    {
        normal.Normalize();
        d=-(normal*referencepoint);
    }
    Vector3 getNormal()const
    {
        return normal;
    }
    bool isnormaldown()const
    {
        Vector3 p=referencepoint;
        p.Translate(normal);
        return ispointlower(p);
    }
    float distancefrom(const Vector3 &v)const
    {
        float a=normal.X();
        float b=normal.Y();
        float c=normal.Z();
        return fabs((normal*v)+d)/sqrt(a*a+b*b+c*c);
    }
    Vector3 getapoint()const
    {
        return referencepoint;
    }
    void reverse()
    {
        normal=-normal;
        d=-d;
    }

    float getD() const
    {
        return d;
    }
    Vector3 projectonplane(const Vector3 &v)const
    {
        Vector3 p=referencepoint+v;
		Vector3 nor = normal*v.length()*Vector3::abscosbetween(v, normal);
        Vector3 p1=p+nor;
        if(!ispointin(p1))
            p1=p-nor;
        Vector3 out(p1-referencepoint);
        return Vector3(out.X(),out.Y(),out.Z());
    }
    void Translate(const Vector3 &v)
    {
        referencepoint.Translate(v);
        d=(-normal*referencepoint);
    }
    void Rotate(const Vector3 p1,const Vector3 &p2,const float &theta)
    {
        referencepoint.Rotate(p1,p2,theta);
        normal.RotateAsVector(p1,p2,theta);
        d=(-normal*referencepoint);
    }
    bool ispointin(const Vector3 &p)const
    {
        float c=(p*normal)+d;
        return (floatsequal(c,0));
    }
    bool ispointupper(const Vector3 &p)const
    {
        float c=(p*normal)+d;
        return c>0;
    }
    bool ispointlower(const Vector3 &p)const
    {
        float c=(p*normal)+d;
        return c<0;
    }
};

#endif // PLANE_H

