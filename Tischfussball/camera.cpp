#include "camera.h"

#define SQR(x) (x*x)

#define NULL_VECTOR Vector3()



/***************************************************************************************/

Camera::Camera(const Vector3 &Start)
    :Position(Start), View(Vector3(0.0f, 0.0f, 1.0f))
    , RightVector(Vector3(1.0f, 0.0f, 0.0f)), Up(Vector3(0.0f, 1.0f, 0.0f))
    , RotatedX(0.0f), RotatedY(0.0f), RotatedZ(0.0f)
{

}

void Camera::Move (const Vector3 &Direction)
{
    Position += Direction;
}

void Camera::RotateX (const GLdouble &Angle)
{

	RotatedX += Angle;    
	//Rotate viewdir around the right vector:
    View = (View*cosf(Angle*PIdiv180)+ Up*sinf(Angle*PIdiv180));
    View.Normalize();
	//now compute the new UpVector (by cross product)
    Up = Vector3::crossproduct(View, RightVector);
}

void Camera::RotateY (const GLdouble &Angle)
{
	RotatedY += Angle;
	//Rotate viewdir around the up vector:
    View = View*cosf(Angle*PIdiv180)- RightVector*sinf(Angle*PIdiv180);
    View.Normalize();
	//now compute the new RightVector (by cross product)
    RightVector = Vector3::crossproduct(View, Up)*-1;
}

void Camera::RotateZ(const GLdouble &Angle)
{
    RotatedZ += Angle;
    //Rotate viewdir around the right vector:
    RightVector = RightVector*cosf(Angle*PIdiv180)+ Up*sinf(Angle*PIdiv180);
    RightVector.Normalize();
    //now compute the new UpVector (by cross product)
    Up = Vector3::crossproduct(View, RightVector);
}

void Camera::Render() const
{
	//calculate view point
    Vector3 ViewPoint = Position+View;


    gluLookAt(	Position.X(),Position.Y(),Position.Z(),
                ViewPoint.X(),ViewPoint.Y(),ViewPoint.Z(),
                Up.X(),Up.Y(),Up.Z());

}

void Camera::MoveForward(const GLdouble &Distance )
{
    Position += (View*Distance);
}

void Camera::MoveRight (const GLdouble &Distance )
{
    Position += (RightVector*Distance);
}

void Camera::MoveUpward(const GLdouble &Distance )
{
    Position += (Up*Distance);
}

void Camera::SetRotateX(const GLdouble &Angle)
{

    View = Vector3( 0.0f, 0.0f, -1.0f);
    RightVector = Vector3(1.0f, 0.0f, 0.0f);
    Up = Vector3(0.0f, 1.0f, 0.0f);
	RotatedX=Angle;
}
