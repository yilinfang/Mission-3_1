#include "HeavyPlane.h"
#include <windows.h>
#include <GL/glut.h>
HeavyPlane::HeavyPlane(float x,float y, float v)
{
    //ctor

    Init(x,y,v);
}

void HeavyPlane::Init(float x,float y, float v)
{
    Plane::Init(x,y,v);
    //InitBase();
    setHp(100);
    //setSpeed(-0.1f);
    setlowSpeedState(FALSE);
    setlowSpeedTime(0);
}
void HeavyPlane::Display()
{
    glColor3f(0.0f,0.0f,1.0f);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(ctPos.x,ctPos.y);
    glEnd();
}
void HeavyPlane::LaserAttack()
{
    hp -= 25;
}
void HeavyPlane::EMPAttack()
{
    hp -= 50;
}
void HeavyPlane::lowSpeedAttack(TICK t)
{
    if(!lowSpeedState){
        speed=speed/2;
        lowSpeedTime=t;
        lowSpeedState=TRUE;
    }
    else lowSpeedTime = t;
}
void HeavyPlane::recoverSpeed(TICK t)
{
    if (lowSpeedState && (t - lowSpeedTime) >= 2000)
    {
        speed=2*speed;
        lowSpeedTime=0;
        lowSpeedState=false;
    }
}
HeavyPlane::~HeavyPlane()
{
    //dtor
}
