#include "wall.h"


Wall::Wall(const float &thewidth, const float &theheight, const float &thedipth, const float &themass, Vector3 thecenter,const Line &rot,const bool &fr,const Vector3 &frt,Body *o, GLuint *tids)
:Body(themass,origin? thecenter:0, rot, fr, frt, o), dipth(thedipth), width(thewidth), height(theheight)
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

void Wall::draw(const Vector3 &color) 
{
    glPushMatrix();
    glColor3fv(color.toArray());
	if (!origin)
	{
		/*Vector3 b = rotationaxis.getBegin();
		Vector3 e = rotationaxis.getEnd() - b;
		glTranslatef(-b.X(), -b.Y(), -b.Z());

		glRotatef(thetarotate * 180 / PI, e.X(), e.Y(), e.Z());
		glTranslatef(b.X(), b.Y(), b.Z());

		glColor3fv(color.toArray());
		  */
	}
	else
		glTranslatef(centerofmass.X(), centerofmass.Y(), centerofmass.Z());
	up->draw(color);
	ground->draw(color);
	left->draw(color);
	right->draw(color);
	front->draw(color);
	back->draw(color);
    glPopMatrix();
}



bool Wall::operator ==(const Body *b) const
{
    const Wall *w=dynamic_cast<const Wall*>(b);
    return (w)&&(Body::operator ==(b))&&(w->dipth==dipth)&&(w->width==width)&&(w->height==height);
}
