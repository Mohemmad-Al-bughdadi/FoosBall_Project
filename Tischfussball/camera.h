//***************************************************************************
//  Camera based on Vectors
//***************************************************************************
#ifndef CAMERA_H
#define CAMERA_H
#include "vector3.h"
#include <cmath>
#ifdef __linux__
#include <GL/freeglut.h>
#else
#include <windows.h>
#include <gl\glut.h>
#endif

/////////////////////////////////
//Note: All angles in degrees  //
/////////////////////////////////

class Camera
{
public:
    Vector3 Position;
    Vector3 View;
    Vector3 RightVector;
    Vector3 Up;


	double RotatedX, RotatedY, RotatedZ;

public:
    Camera(const Vector3 &Start);				//inits the values (Position: (0|0|0) Target: (0|0|-1) )
    void Render()const;	//executes some glRotates and a glTranslate command
							//Note: You should call glLoadIdentity before using Render

    void Move (const Vector3 &Direction );
    void RotateX (const GLdouble &Angle );
    void RotateY (const GLdouble &Angle );
    void RotateZ (const GLdouble &Angle );

    void MoveForward (const GLdouble &Distance );
    void MoveUpward (const GLdouble &Distance );
    void MoveRight (const GLdouble &Distance );

    void SetRotateX(const GLdouble &Angle);


};
#endif
