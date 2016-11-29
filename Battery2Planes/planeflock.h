#ifndef PLANEFLOCK_H_INCLUDED
#define PLANEFLOCK_H_INCLUDED
#include "plane.h"
#include "mutex.h"
class PlaneNode
{
public:
    PlaneNode()
    {
        pdata = NULL;
        next = NULL;
    }
    ~PlaneNode()
    {
        if(pdata!=NULL)
        {
            delete pdata;
            pdata = NULL;
        }
     }
public:
    Plane* pdata;
    PlaneNode*next;
};

class PlaneFlock{
public:
    PlaneFlock();
    ~PlaneFlock();

    void Init();
    void Release();

    void LaserShoot();//�������
    void EMPShoot();//EMPը��
    void lowSpeedAttack(TICK t);//���ٵ�����
    void recoverSpeed(TICK t);
    void RemoveDeadPlanes();//��liveHead �������Ƴ�hp<=0�ķɻ���deadHead��ȥ
    PlaneNode*SearchNearestPlane();//Ѱ������ɻ�����

    void addHeavyPlane(int n,float x=0.8f,float y=0.0f,float v=-0.1f); //���n�����ͷɻ�
    void addLightPlane(int n,float x=0.8f,float y=0.0f,float v=-0.2f); //���n�����ͷɻ�
    void addRedTriPlane(int n,float x=0.8f,float y=0.0f,float v=-0.3f);//���n�ܺ����Ƿɻ�


    PlaneNode*getHeavyFromDead(float x,float y,float v);//��deadHead�õ�һ�����ͷɻ�,ͬʱ��������ɾ��;���û���򷵻�NULL
    PlaneNode*getLightFromDead(float x,float y,float v);
    PlaneNode*getRedTriFromDead(float x,float y,float v);
    void addHeavyToDead(PlaneNode*p);//�ɻ�p��ָ��Ľڵ����deadHead������ȥ

    bool PlaneWin();

    int Length(PlaneNode*h); //FOR debug
    void PrintPlanes(); //For debug



    void UpdatePosition(TICK dt);





    void planesDisplay();


    bool Result()
    {
       return 0;
    }

private:
    PlaneNode*liveHead; //���ڷ��еķɻ�����,liveHeadָ��ͷ�ڵ�
    PlaneNode*deadHead; //����ķɻ��أ����ڻ��շɻ�,deadHeadָ��ͷ�ڵ�

    Mutex mt;//���������
};

#endif // PLANEFLOCK_H_INCLUDED
