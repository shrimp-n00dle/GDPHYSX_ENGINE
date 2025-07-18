#pragma once
#include "ForceGenerator.h"

namespace P6
{
    class AnchorSprings :public ForceGenerator
    {
    private:
        //point hwere the other end of the spring is
        MyVector anchorPoint;

        //how stiff the spring is
        float springConstant;

        //length of the spring
        float restLength;

    public:
        AnchorSprings(MyVector pos, float _springConst, float _restLen) :
            anchorPoint(pos), springConstant(_springConst), restLength(_restLen) {};

        //override
        void updateForce(MyParticle* particle, float time) override;

    };
}

