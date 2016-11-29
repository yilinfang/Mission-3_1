#ifndef HEAVYPLANE_H
#define HEAVYPLANE_H

#include "plane.h"


class HeavyPlane : public Plane
{
    public:
        HeavyPlane(float x=0.8f,float y=0.0f, float v=-1.0f);
        void Init(float x=0.8f,float y=0.0f, float v=-1.0f);
        void Display();
        virtual ~HeavyPlane();
        void LaserAttack();
        void EMPAttack();
        void lowSpeedAttack(TICK t);
        void recoverSpeed(TICK t);
    protected:

    private:
};

#endif // HEAVYPLANE_H
