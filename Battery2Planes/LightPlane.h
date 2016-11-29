#ifndef LIGHTPLANE_H
#define LIGHTPLANE_H

#include "Plane.h"


class LightPlane : public Plane
{
    public:
        LightPlane(float x=0.8f,float y=0.0f, float v=-1.0f);
        virtual ~LightPlane();
        virtual void Display();
        void LaserAttack();
        void EMPAttack();
        void lowSpeedAttack(TICK t);
        void recoverSpeed(TICK t);
        void Init(float x=0.8f,float y=0.0f, float v=-1.0f);

    protected:

    private:
};

#endif // LIGHTPLANE_H
