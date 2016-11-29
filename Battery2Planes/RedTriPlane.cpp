#include "RedTriPlane.h"
#include <windows.h>
#include <GL/glut.h>
RedTriPlane::RedTriPlane(float x,float y, float v)
{
    //ctor

    Init(x,y,v);

}
void RedTriPlane::Display()
{
    glColor3f(1.0f,0.0f,0.0f);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(ctPos.x,ctPos.y);
    glEnd();

}
void RedTriPlane::Init(float x,float y, float v)
{

     Plane::Init(x,y,v);
     //InitBase();
     //setSpeed(-0.3f);
     setHp(150);
     setlowSpeedState(FALSE);
     setlowSpeedTime(0);
}
void RedTriPlane::LaserAttack()
{
    hp -= 75;
}
void RedTriPlane::EMPAttack()
{
    hp -= 75;
}
void RedTriPlane::lowSpeedAttack(TICK t)
{
    if (!lowSpeedState){
        lowSpeedState=TRUE;
        speed=speed/2;
        lowSpeedTime=t;
    }
    else lowSpeedTime=t;
}
void RedTriPlane::recoverSpeed(TICK t)
{
    if(lowSpeedState && (t - lowSpeedTime) >= 2000){
        lowSpeedState=FALSE;
        speed=speed*2;
        lowSpeedTime = 0;
    }
}
RedTriPlane::~RedTriPlane()
{
    //dtor
}
