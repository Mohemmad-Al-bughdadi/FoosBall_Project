#include "wall.h"


Wall::Wall(const double &thewidth, const double &theheight, const double &thedipth, const double &themass, Vector3 thecenter,const Line &rot,const bool &fr,const Vector3 &frt,Body *o, GLuint *tids)
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

void Wall::Rotate()
{
    Vector3 begin=rotationaxis.getBegin(),end=rotationaxis.getEnd();
    up->Rotate(begin,end,thetarotate);
    ground->Rotate(begin,end,thetarotate);
    right->Rotate(begin,end,thetarotate);
    left->Rotate(begin,end,thetarotate);
    front->Rotate(begin,end,thetarotate);
    back->Rotate(begin,end,thetarotate);
}

void Wall::proceedintime()
{
    Vector3 diff=centerofmass;
    Body::proceedintime();
    diff=centerofmass-diff;
    Translate(diff);
    Rotate();
}



bool Wall::operator ==(const Body *b) const
{
    const Wall *w=dynamic_cast<const Wall*>(b);
    return (w)&&(Body::operator ==(b))&&(w->dipth==dipth)&&(w->width==width)&&(w->height==height);
}
