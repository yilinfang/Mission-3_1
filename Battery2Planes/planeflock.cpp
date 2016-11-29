#include <stdlib.h>
#include <stdio.h>
#include "planeflock.h"
#include "plane.h"
#include "HeavyPlane.h"
#include "LightPlane.h"
#include "RedTriPlane.h"
#include <typeinfo>
#include <GL/glut.h>


PlaneFlock::PlaneFlock()
{
    Init();
}

PlaneFlock::~PlaneFlock()
{
    Release();
}

void PlaneFlock::planesDisplay()
{
    PlaneNode *p=liveHead->next;
    Pos2D pos;
    mt.Lock();
    glBegin(GL_POINTS);
    while (p!=NULL)
    {
        p->pdata->Display();
        p=p->next;
    }
    glEnd();
    mt.UnLock();
}

void PlaneFlock::Init()
{
    mt.Lock();
    liveHead = new PlaneNode(); //创建头节点
    deadHead = new PlaneNode();
    mt.UnLock();
}

void PlaneFlock::Release()//释放两个链表的节点空间
{
    PlaneNode *p;
    mt.Lock();
    while(liveHead!=NULL)
    {
        p = liveHead;
        liveHead = liveHead->next;
        delete p;
    }

    while(deadHead!=NULL)
    {
        p = deadHead;
        deadHead = deadHead->next;
        delete p;
    }
    mt.UnLock();

}

void PlaneFlock::UpdatePosition(TICK dt) //更新位置
{

    mt.Lock();
    PlaneNode *p=liveHead->next;
    while(p!=NULL)
    {
       p->pdata->UpdatePos(dt);
       p=p->next;
    }
    mt.UnLock();
}


void PlaneFlock::addHeavyPlane(int n,float x,float y,float v) //添加n架重型飞机
{
    int i=0;
    PlaneNode*p;
    mt.Lock();
    while(i<n)
    {
        p = getHeavyFromDead(x,y,v);
        p->next = liveHead->next;
        liveHead->next = p;
        i++;
    }
    mt.UnLock();
 }
void PlaneFlock::addLightPlane(int n,float x,float y,float v) //添加n架轻型飞机
{
    int i=0;
    PlaneNode*p;
    mt.Lock();
    while(i<n)
    {
        p = getLightFromDead(x,y,v);
        p->next = liveHead->next;
        liveHead->next = p;
        i++;
    }
    mt.UnLock();
}
void PlaneFlock::addRedTriPlane(int n,float x,float y, float v) //添加n架红三角飞机
{
    int i=0;
    PlaneNode*p;
    mt.Lock();
    while(i<n)
    {
        p = getRedTriFromDead(x,y,v);
        p->next = liveHead->next;
        liveHead->next = p;
        i++;
    }
    mt.UnLock();
}
PlaneNode*PlaneFlock::getHeavyFromDead(float x,float y, float v)//从deadHead得到一架重型飞机,同时从链表中删除;如果没有则返回NULL
{
    mt.Lock();
    PlaneNode*prev=deadHead;
    PlaneNode*current=prev->next;

    while(current!=NULL)
    {
        if(typeid(*current->pdata)==typeid(HeavyPlane))  //判别是哪种飞机类型
        {
            //printf("find Heavy Plane\n");
            prev->next = current->next;//删除该节点
            current->pdata->Init(x,y,v);//恢复初始数据
            mt.UnLock();
            return current;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
    mt.UnLock();
    PlaneNode*p = new PlaneNode();
    p->pdata= new HeavyPlane(x,y,v);
    return p; //创建一个新的

}
PlaneNode*PlaneFlock::getRedTriFromDead(float x,float y, float v)//从deadHead得到一架红三角飞机,同时从链表中删除;如果没有则返回NULL
{
    mt.Lock();
    PlaneNode*prev=deadHead;
    PlaneNode*current=prev->next;

    while(current!=NULL)
    {
        if(typeid(*current->pdata)==typeid(RedTriPlane))  //判别是哪种飞机类型
        {
            //printf("find Heavy Plane\n");
            prev->next = current->next;//删除该节点
            current->pdata->Init(x,y,v);//恢复初始数据
            mt.UnLock();
            return current;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
    mt.UnLock();
    PlaneNode*p = new PlaneNode();
    p->pdata= new RedTriPlane(x,y,v);
    return p; //创建一个新的
}
bool PlaneFlock::PlaneWin()
{
     PlaneNode* p= SearchNearestPlane();

     if(p!=NULL && p->pdata->getHp()>0 && (p->pdata->getPosition().x < -0.8f))
          return true;
      return false;
}
PlaneNode*PlaneFlock::getLightFromDead(float x,float y, float v)//从deadHead得到飞机,同时从链表中删除;如果没有则返回NULL
{
    mt.Lock();
    PlaneNode*prev=deadHead;
    PlaneNode*current=prev->next;

    while(current!=NULL)
    {
        if(typeid(*current->pdata)==typeid(LightPlane))
        {
            prev->next = current->next;//删除该节点
            current->pdata->Init(x,y,v);  //恢复初始数据
            //printf("pos=%f\n",current->pdata->getPosition().x);
            mt.UnLock();
            return current;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
    mt.UnLock();
    PlaneNode*p = new PlaneNode();
    p->pdata= new LightPlane(x,y,v);
    return p; //创建一个新的

}
void PlaneFlock::addHeavyToDead(PlaneNode*p)//飞机p所指向的节点加入deadHead链表中去
{
    mt.Lock();
    p->next = deadHead->next;  //增加在第1个节点
    deadHead->next = p;
    mt.UnLock();

}

int PlaneFlock::Length(PlaneNode*h)
{
    int n=0;
    mt.Lock();
    PlaneNode*p=h->next;
    while(p!=NULL)
    {
        n++;
    }
    mt.UnLock();
    return n;

}
void PlaneFlock::PrintPlanes()
{
    mt.Lock();
    PlaneNode*p=liveHead->next;
    printf("\nlivehead=");
    while(p!=NULL)
    {
        printf("%d,",p->pdata->getHp());
        p = p->next;
    }
    printf("\ndeadhead=");
    p=deadHead->next;
    while(p!=NULL)
    {
        printf("%d,",p->pdata->getHp());
        p=p->next;
    }
    mt.UnLock();


}


void PlaneFlock::LaserShoot()
{

    mt.Lock();
    PlaneNode*p=SearchNearestPlane();
    if(p!=NULL)
        p->pdata->LaserAttack();
    mt.UnLock();
}

void PlaneFlock::EMPShoot()
{

    mt.Lock();
    PlaneNode*p=liveHead->next;
    while(p!=NULL)
    {
        p->pdata->EMPAttack();
        p=p->next;
    }
    mt.UnLock();

}

void PlaneFlock::lowSpeedAttack(TICK t)
{
    mt.Lock();
    PlaneNode*p=liveHead->next;
    while(p!=NULL)
    {
        p->pdata->lowSpeedAttack(t);
        p=p->next;
    }
    mt.UnLock();
}

void PlaneFlock::recoverSpeed(TICK t)
{
    mt.Lock();
    PlaneNode*p=liveHead->next;
    while(p!=NULL)
    {
        p->pdata->recoverSpeed(t);
        p=p->next;
    }
    mt.UnLock();
}

void PlaneFlock::RemoveDeadPlanes()//从liveHead 队列中移除hp<=0的飞机到deadHead中去
{


    mt.Lock();

    PlaneNode*prev=liveHead;
    PlaneNode*current=prev->next;

    while(current!=NULL)
    {

        if(current->pdata->getHp()<=0)
        {


            prev->next = current->next;  //删除

            current->next = deadHead->next;//加入到deadHead表中去
            deadHead->next = current;

            current = prev->next;  //恢复指针到prev的下一个
        }
        else
        {
            prev = current;
            current = prev->next;
        }
    }
    mt.UnLock();


 }

 PlaneNode* PlaneFlock::SearchNearestPlane()//寻找最近飞机返回
 {
    mt.Lock();
    PlaneNode*p=liveHead->next;
    PlaneNode*minp;
    if(p==NULL)
    {
        mt.UnLock();
        return NULL;
    }

    minp=p;
    p=p->next;
    while(p!=NULL)
    {
        if(p->pdata->getPosition().x < minp->pdata->getPosition().x)
            minp = p;
        p=p->next;
    }
    mt.UnLock();
    return minp;

 }

