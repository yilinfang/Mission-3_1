#include<iostream>
#include "plane.h"
#include <windows.h>
#include <GL/glut.h>
Plane::Plane(float x, float y,float v)
{
    Init(x,y,v);
}
 Plane::~Plane()
 {

 }

void Plane::Init(float x,float y,float v)
{
    float rx,ry;
    rx = (2.0*rand()/(RAND_MAX+1.0) - 1)*0.05f;
    ry = (2.0*rand()/(RAND_MAX+1.0) - 1)*0.05f;


    stPos = Pos2D(x+rx,y+ry);//飞机的起点,(x,y)为圆心0.05为半径内的随机点
    ctPos = stPos;  //当前位置

    birth = GetTickCount();
    speed = v;
    hp = 0;//血量
}
void Plane::setBirth(DWORD t)//帧
{
    birth=t;
}
void Plane::setSpeed(float t)
{
    speed=t;
}
void Plane::setHp(int t)
{
    hp=t;
}
void Plane::setlowSpeedState(bool t)
{
    lowSpeedState = t;
}
void Plane::setlowSpeedTime(TICK t)
{
    lowSpeedTime=t;
}
Pos2D Plane:: getPosition()
{
    return ctPos;
}
float Plane::getSpeed()
{
    return speed;
}
TICK Plane::getBirth()
{
    return birth;
}
int Plane::getHp()
{
    return hp;
}

bool Plane::getlowSpeedState()
{
    return lowSpeedState;
}
TICK Plane::getlowSpeedTime()
{
    return lowSpeedTime;
}
void Plane::UpdatePos(TICK dt)//根据当前时间计算当前位置
{
    ctPos.x = ctPos.x + speed*dt/1000.0f;
}


void Plane::Display()
{
    //std::cout<<"位置："<<ctPos.x<<" 出现时间："<<birth<<" 速度："<<speed<<" "<<std::endl;
    //glVertex2f(-0.5+(flock[i].getPosition())/100.0 + 0.01*c2,0.05*c1);

    glColor3f(1.0f,1.0f,1.0f);
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex2f(ctPos.x,ctPos.y);
    glEnd();

}









    //void Display();
