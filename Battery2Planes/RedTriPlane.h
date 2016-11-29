#ifndef REDTRIPLANE_H_INCLUDED
#define REDTRIPLANE_H_INCLUDED

#include "Plane.h"


class RedTriPlane : public Plane
{
    public:
        RedTriPlane(float x=0.8f,float y=0.0f, float v=-1.0f);
        virtual ~RedTriPlane();
        virtual void Display();
        void LaserAttack();
        void EMPAttack();
        void lowSpeedAttack(TICK t);
        void recoverSpeed(TICK t);
        void Init(float x=0.8f,float y=0.0f, float v=-1.0f);
    protected:
    private:
};

#endif // REDTRIPLANE_H_INCLUDED
