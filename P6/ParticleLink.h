#pragma once

#include "MyParticle.h"
#include "ParticleContact.h"

namespace P6
{
	class ParticleLink
	{
	public:
		//particles[0] = anchorParticles
		//particles[1] = pendulumParticles
		MyParticle* particles[2];

		virtual ParticleContact* GetContact() { return nullptr; };

	protected:
		//used mainly for interpenetration
		float currentLength();

	};
}

