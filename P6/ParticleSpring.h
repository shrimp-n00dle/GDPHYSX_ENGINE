#pragma once
#include "ForceGenerator.h"

namespace P6 
{
	class ParticleSpring : public ForceGenerator
	{
		private:
			//other particle the spring is attached to
			MyParticle* otherParticle;

			//how stiff spring is
			float springConstant;

			//rest length of string
			float restLength;

		public:

			ParticleSpring(MyParticle* particle, float _springConst, float _restLen) : 
				otherParticle(particle), springConstant(_springConst), restLength(_restLen) {}

			//override
			void updateForce(MyParticle* particle, float time) override;

			void chainUpdate(MyParticle* particle, float time);

	};
}


