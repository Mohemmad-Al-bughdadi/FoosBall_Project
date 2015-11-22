#ifndef LINE_H
#define LINE_H
#include "vector3.h"
#include "plane.h"
struct Line
{
private:
	Vector3 begin, end;
	Vector3 orentation;
public:
	Line(const Vector3 &b, const Vector3 &e) :begin(b), end(e), orentation(e - b)
	{

	}
	Vector3 projectonline(const Vector3 &v)const
	{
		Plane p(begin,end,v);
		Vector3 nor=Vector3::crossproduct(p.getNormal(),orentation);
		Vector3 out=v+nor;
		if(!ispointin(out))
			out=v-nor;
		return out;
	}
    bool notaline()const
    {
        return (begin==end);
    }
	double distancefrom(const Vector3 &v)const
	{
		return Vector3::crossproduct(end - begin, begin - v).length() / (end - begin).length();
	}
    Line(const Line &l) :begin(l.begin), end(l.end),orentation(l.orentation)
	{

	}
	Vector3 getBegin() const
	{
		return begin;
	}
	void setBegin(const Vector3 &value)
	{
		begin = value;
		orentation = end - begin;
	}

	Vector3 getEnd() const
	{
		return end;
	}
	void setEnd(const Vector3 &value)
	{
		end = value;
		orentation = end - begin;
	}
	Vector3 getOrentiation() const
	{
		return orentation;
	}        
    bool ispointin(const Vector3 &p)const
    {
        double a=orentation.X(),b=orentation.Y(),c=orentation.Z();
        double x0=begin.X(),y0=begin.Y(),z0=begin.Z();
        double x=p.X(),y=p.Y(),z=p.Z();
        if(roundf(a)==0)
        {
            if(x!=x0)
                return false;
            else if(roundf(b)==0)
            {
                if(y!=y0)
                    return false;
                else if(roundf(c)==0)
                {
                    if(z!=z0)
                        return false;
                }
                return true;
            }
            else
            {
                if(roundf(c)==0)
                {
                    if(z!=z0)
                        return false;
                    return true;
                }
                else
                {
                    return ((y-y0)/b)==((z-z0)/c);
                }
            }
        }
        else
        {
            if(roundf(b)==0)
            {
                if(y!=y0)
                    return false;
                else if(roundf(c)==0)
                {
                    if(z!=z0)
                        return false;
                    return true;
                }
                return ((x-x0)/a)==((z-z0)/c);
            }
            else
            {
                if(roundf(c)==0)
                {
                    if(z!=z0)
                        return false;
                    return ((x-x0)/a)==((y-y0)/b);
                }
                else
                {
                    return (((y-y0)/b)==((z-z0)/c))&&(((x-x0)/a)==((y-y0)/b));
                }
            }
        }
    }
	/*static bool isperpendicular(const Line &l1,const Line &l2)
	{
	return Vector3::isperpendicular(l1.getOrentiation(),l2.getOrentiation());
	}
	static bool isparallel(const Line &l1,const Line &l2)
	{
	return Vector3::isparallel(l1.getOrentiation(),l2.getOrentiation());
	}
	static Vector3 *intersect(const Line &l1,const Line &l2)
	{

	}*/
};


#endif // LINE_H
