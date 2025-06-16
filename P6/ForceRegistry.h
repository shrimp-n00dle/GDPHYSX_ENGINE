#pragma once

#include "EngineParticle.h"
#include "ForceGenerator.h"

#include "list"


namespace P6
{
	class ForceRegistry
	{
	public:
		
		void Add(EngineParticle* particle, ForceGenerator* generator);
		void Remove(EngineParticle* particle, ForceGenerator* generator);
		void Clear();
		void updateForces(float time);
	protected:
		/*Holds the references for the Particle and Generator pair
		Does not have any other use*/
		struct ParticleForceRegistry
		{
			EngineParticle* particle;
			ForceGenerator* generator;
		};

		std::list<ParticleForceRegistry> Registry;
	};
}


