#ifndef FORCE_H
#define FORCE_H
#include "vector3.h"

struct Force
{
private :
    float strength;
    Vector3 orentation;
public :
    Force(const Vector3 &v,const float &s)
        :strength(s),orentation(v)
    {
        orentation.Normalize();
    }
    Force(const Force &f)
        :strength(f.strength),orentation(f.orentation)
    {

    }

    Force operator -()const
    {
        return Force(-orentation,strength);
    }

    Force operator +(const Force &f)const
    {
        Force tot(f.orentation+orentation,0);
        tot.strength = sqrt(f.strength*f.strength + strength*strength + 2 * strength*f.strength*cos(Vector3::anglebetweeninradian(orentation, f.orentation)));
        return tot;
    }
    Force operator -(const Force &f)const
    {
        Force tot(orentation-f.orentation,0);
        tot.strength = sqrt(f.strength*f.strength + strength*strength - 2 * strength*f.strength*cos(Vector3::anglebetweeninradian(orentation, f.orentation)));
        return tot;
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
