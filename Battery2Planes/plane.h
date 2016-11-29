#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

typedef unsigned long TICK;

class Pos2D
{
public:
    Pos2D()
    {
        x=y=0.0f;
    }
    Pos2D(float x,float y)
    {
        this->x = x;
        this->y = y;
    }
public:
    float x;  //屏幕坐标系坐标
    float y;
};

class Plane{
public:
    Plane(float x=0.8f,float y=0.0f,float v=-1.0f);
    virtual void LaserAttack()=0;//激光攻击
    virtual void EMPAttack()=0;//EMP炸弹攻击
    virtual void lowSpeedAttack(TICK t)=0;//减速弹攻击
    virtual void recoverSpeed(TICK t)=0;
    virtual ~Plane();
    virtual void Init(float x=0.8f,float y=0.0f,float v=-1.0f);
    virtual void Display();
    void setBirth(TICK t);
    void setSpeed(float t);
    void setHp(int t);
    void setlowSpeedState(bool t);
    void setlowSpeedTime(TICK t);
    bool getlowSpeedState();
    TICK getlowSpeedTime();
    void UpdatePos(TICK dt);//根据当前时间计算当前位置
    Pos2D getPosition();
    float getSpeed();
    TICK getBirth();
    int getHp();

protected:
    bool lowSpeedState;
    TICK lowSpeedTime;
    Pos2D stPos;  //飞机的起点
    Pos2D ctPos;  //当前位置
    TICK birth;//飞机的生成时间,通过getTickCount取得
    float speed;
    int hp;//血量

};


#endif // PLANE_H_INCLUDED
