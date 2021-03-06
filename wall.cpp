#include "wall.h"


void Wall::checknormals()
{
    ground->checknormal(centerofmass);
    left->checknormal(centerofmass);
    front->checknormal(centerofmass);
    right->checknormal(centerofmass);
    back->checknormal(centerofmass);
    up->checknormal(centerofmass);
}

Wall::Wall(const float &thewidth, const float &theheight, const float &thedipth, const float &themass, Vector3 thecenter,const Line &rot,const bool &fr,const Vector3 &frt,Body *o, GLuint *tids)
:Body(themass,thecenter, rot, fr, frt, o), dipth(thedipth), width(thewidth), height(theheight)
{
    inertia=themass*((theheight*theheight)+(thedipth*thedipth))/12;
ground = new Quad(thecenter+Vector3( - width / 2,- height / 2,  - dipth / 2),
	thecenter + Vector3(-width / 2, -height / 2, +dipth / 2),
	thecenter + Vector3(+width / 2, -height / 2, +dipth / 2),
	thecenter + Vector3(+width / 2, -height / 2, -dipth / 2), tids != NULL ? tids[0] : 0);
left = new Quad(thecenter + Vector3(-width / 2, +height / 2, -dipth / 2),
	thecenter + Vector3(-width / 2, -height / 2, -dipth / 2),
	thecenter + Vector3(-width / 2, -height / 2, +dipth / 2),
	thecenter + Vector3(-width / 2, +height / 2, +dipth / 2), tids != NULL ? tids[1] : 0);
front = new Quad(thecenter + Vector3(-width / 2, +height / 2, -dipth / 2),
	thecenter + Vector3(-width / 2, -height / 2, -dipth / 2),
	thecenter + Vector3(+width / 2, -height / 2, -dipth / 2),
	thecenter + Vector3(+width / 2, +height / 2, -dipth / 2), tids != NULL ? tids[2] : 0);

right = new Quad(thecenter + Vector3(+width / 2, +height / 2, -dipth / 2),
	thecenter + Vector3(+width / 2, -height / 2, -dipth / 2),
	thecenter + Vector3(+width / 2, -height / 2, +dipth / 2),
	thecenter + Vector3(+width / 2, +height / 2, +dipth / 2), tids != NULL ? tids[3] : 0);



back = new  Quad(thecenter + Vector3(-width / 2, +height / 2, +dipth / 2),
	thecenter + Vector3(-width / 2, -height / 2, +dipth / 2),
	thecenter + Vector3(+width / 2, -height / 2, +dipth / 2),
	thecenter + Vector3(+width / 2, +height / 2, +dipth / 2), tids != NULL ? tids[4] : 0);

up = new	Quad(thecenter + Vector3(-width / 2, +height / 2, -dipth / 2),
	thecenter + Vector3(-width / 2, +height / 2, +dipth / 2),
	thecenter + Vector3(+width / 2, +height / 2, +dipth / 2),
	thecenter + Vector3(+width / 2, +height / 2, -dipth / 2), tids != NULL ? tids[5] : 0);

    checknormals();

}

void Wall::draw(const Vector3 &color) const
{    
	up->draw(color);
	ground->draw(color);
    right->draw(color);
    left->draw(color);
	front->draw(color);
    back->draw(color);
}

void Wall::Translate(const Vector3 &diff)
{
    up->Translate(diff);
    ground->Translate(diff);
    right->Translate(diff);
    left->Translate(diff);
    front->Translate(diff);
    back->Translate(diff);
}

void Wall::Rotate(const float &diff)
{
    Vector3 begin=rotationaxis.getBegin(),end=rotationaxis.getEnd();
    up->Rotate(begin,end,diff);
    ground->Rotate(begin,end,diff);
    right->Rotate(begin,end,diff);
    left->Rotate(begin,end,diff);
    front->Rotate(begin,end,diff);
    back->Rotate(begin,end,diff);
}

void Wall::proceedintime()
{
    Vector3 diff=centerofmass;
    float diffrot=thetarotate;
    Body::proceedintime();
    diff=centerofmass-diff;
    diffrot=thetarotate-diffrot;
    Translate(diff);
    Rotate(diffrot);
}



bool Wall::operator ==(const Body *b) const
{
    const Wall *w=dynamic_cast<const Wall*>(b);
    return (w)&&(Body::operator ==(b))&&(w->dipth==dipth)&&(w->width==width)&&(w->height==height);
}
