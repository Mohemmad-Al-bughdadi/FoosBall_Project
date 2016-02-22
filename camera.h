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


	float RotatedX, RotatedY, RotatedZ;

public:
    Camera(const Vector3 &Start);				//inits the values (Position: (0|0|0) Target: (0|0|-1) )
    void Render()const;	//executes some glRotates and a glTranslate command
							//Note: You should call glLoadIdentity before using Render

    void Move (const Vector3 &Direction );
    void RotateX (const GLfloat &Angle );
    void RotateY (const GLfloat &Angle );
    void RotateZ (const GLfloat &Angle );

    void MoveForward (const GLfloat &Distance );
    void MoveUpward (const GLfloat &Distance );
    void MoveRight (const GLfloat &Distance );

    void SetRotateX(const GLfloat &Angle);


};
#endif
