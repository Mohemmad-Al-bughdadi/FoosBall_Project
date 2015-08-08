#include "ball.h"
map<BodyPair,float> Ball::RollingFrictionCoffs;
float Ball::getRadius() const
{
    return radius;
}





Ball::Ball(const float &theradius,const Vector3 &origin,const float &mass,const Line &rot,const bool &fr,const Vector3 &tr,const GLuint &theTexure,Body *o)
:Body(mass, origin,rot, fr, tr, o)
    ,radius(theradius), texture_id(theTexure)
{
    inertia=0.4f*mass*theradius*theradius;
}



void Ball::draw(const Vector3 &color)
{
	glPushMatrix();
    glColor3fv(color.toArray());
	/*if (!origin)
	{
		Vector3 b = rotationaxis.getBegin();
		Vector3 e = rotationaxis.getEnd() - b;

		glTranslatef(b.X(), b.Y(), b.Z());

		glRotatef(thetarotate * 180 / PI, e.X(), e.Y(), e.Z());
		glTranslatef(-b.X(), -b.Y(), -b.Z());

	} */
	glTranslatef(centerofmass.X(), centerofmass.Y(), centerofmass.Z());
	GLUquadric * gq =  gluNewQuadric();
	gluQuadricNormals(gq, GLU_SMOOTH);
	gluQuadricTexture(gq, GL_TRUE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_id);
	gluSphere(gq, radius, radius * 30, radius * 20);
	glDisable(GL_TEXTURE_2D);
    glPopMatrix();    
}



bool Ball::operator ==(const Body *b) const
{
    const Ball *p=dynamic_cast<const Ball*>(b);
    return (p)&&(Body::operator ==(b));
}

