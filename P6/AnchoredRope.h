#pragma once
#include "ForceGenerator.h"

namespace P6
{
    class AnchoredRope : public ForceGenerator
    {
    private:
        //Point in the world where the other rope is anchored
        MyVector anchorPoint;

        //rope stiffness
        float ropeConstant;

        //Rest length of the rope
        float restLength;

        //
        float TEMP_DampingFactor;

    public:
        AnchoredRope(MyVector pos, float _ropeConstant, float _restLen) :
            anchorPoint(pos), ropeConstant(_ropeConstant), restLength(_restLen), TEMP_DampingFactor(0.9f) {
        };

        //override
        void updateForce(MyParticle* particle, float time) override;

    };
}

