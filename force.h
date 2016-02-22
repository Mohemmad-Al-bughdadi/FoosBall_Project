#ifndef FORCE_H
#define FORCE_H
#include "vector3.h"

struct Force
{
private :
    float strength;
    Vector3 orentation;
public :
    Force()
        :strength(0),orentation(Vector3())
    {

    }

    Force(const Vector3 &v,const float &s)
        :strength(s),orentation(v)
    {
        orentation.Normalize();
    }
    Force(const Vector3 &v)
        :strength(v.length()),orentation(v)
    {
        orentation.Normalize();
    }

    Force(const Force &f)
        :strength(f.strength),orentation(f.orentation)
    {

    }
    bool iszero()const
    {
        return orentation.iszero()||(floatsequal(strength,0));
    }
    Force operator -()const
    {
        return Force(-orentation,strength);
    }

    Force operator +(const Force &f)const
    {
        Vector3 f1=orentation*strength;
        Vector3 f2=f.orentation*f.strength;
        return Force(f1+f2);
    }
    Force operator -(const Force &f)const
    {
        Vector3 f1=orentation*strength;
        Vector3 f2=f.orentation*f.strength;
        return Force(f1-f2);
    }   
    Force &operator =(const Force &f)
    {
        orentation=f.orentation;
        strength=f.strength;
        return *this;
    }
    Force &operator +=(const Force &f)
    {
        *this=Force(f+*this);
        return *this;
    }
    Force &operator -=(const Force &f)
    {
        *this=Force(*this-f);
        return *this;
    }

    float getStrength() const
    {
        return strength;
    }
    void setStrength(const float &value)
    {
        strength=value;
    }

    Vector3 getOrentation() const
    {
        return orentation;
    }
    void setOrentation(const Vector3 &value)
    {
        orentation=value;
        orentation.Normalize();
    }
    /*float resulttorque(const Vector3 &v,const Line &r)const
    {
        float rt=r.distancefrom(v);
		return rt*strength;
    }*/
};
#endif // FORCE_H
