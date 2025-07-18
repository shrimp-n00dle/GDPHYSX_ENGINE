#pragma once

#include "ParticleLink.h"

namespace P6
{
	class Rod : public ParticleLink
	{
	public:
		float length = 0.1;

		//always 0  becasuse a rod has no bounciness
		float restitution = 0;

		ParticleContact* GetContact() override;
	};
}

