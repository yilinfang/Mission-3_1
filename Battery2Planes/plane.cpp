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


    stPos = Pos2D(x+rx,y+ry);//�ɻ������,(x,y)ΪԲ��0.05Ϊ�뾶�ڵ������
    ctPos = stPos;  //��ǰλ��

    birth = GetTickCount();
    speed = v;
    hp = 0;//Ѫ��
}
void Plane::setBirth(DWORD t)//֡
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
void Plane::UpdatePos(TICK dt)//���ݵ�ǰʱ����㵱ǰλ��
{
    ctPos.x = ctPos.x + speed*dt/1000.0f;
}


void Plane::Display()
{
    //std::cout<<"λ�ã�"<<ctPos.x<<" ����ʱ�䣺"<<birth<<" �ٶȣ�"<<speed<<" "<<std::endl;
    //glVertex2f(-0.5+(flock[i].getPosition())/100.0 + 0.01*c2,0.05*c1);

    glColor3f(1.0f,1.0f,1.0f);
    glPointSize(3);
    glBegin(GL_POINTS);
    glVertex2f(ctPos.x,ctPos.y);
    glEnd();

}









    //void Display();
