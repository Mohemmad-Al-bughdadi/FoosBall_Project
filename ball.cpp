#include "ball.h"
map<BodyPair,float> Ball::RollingFrictionCoffs;
float Ball::getRadius() const
{
    return radius;
}





Ball::Ball(const float &theradius,const Vector3 &origin,const float &mass,const Line &rot,const bool &fr,const Vector3 &tr,const GLuint &theTexure,Body *o)
    :Body(mass, origin, rot,fr,tr,o)
    ,radius(theradius), texture_id(theTexure)
{
    inertia=0.4f*mass*theradius*theradius;
}



void Ball::draw(const Vector3 &color) const
{
	glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glColor3fv(color.toArray());

    Vector3 b = rotationaxis.getBegin();
    Vector3 e = rotationaxis.getEnd() - b;
    glTranslated(b.X(), b.Y(), b.Z());
    glRotated(thetarotate * 180 / PI, e.X(), e.Y(), e.Z());
    glTranslated(-b.X(), -b.Y(), -b.Z());
    glTranslated(centerofmass.X(), centerofmass.Y(), centerofmass.Z());

    glColor3fv(color.toArray());

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,texture_id);

    glutSolidSphere(radius,40,40);

    glPopAttrib();
    glPopMatrix();    
}



bool Ball::operator ==(const Body *b) const
{
    const Ball *p=dynamic_cast<const Ball*>(b);
    return (p)&&(Body::operator ==(b));
}

