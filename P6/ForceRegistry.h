#pragma once

#include "MyParticle.h"
#include "ForceGenerator.h"

#include "list"


namespace P6
{
	class ForceRegistry
	{
	public:
		
		void Add(MyParticle* particle, ForceGenerator* generator);
		void Remove(MyParticle* particle, ForceGenerator* generator);
		void Clear();
		void updateForces(float time);
	protected:

		/*Holds the references for the Particle and Generator pair
		Does not have any other use*/
		struct ParticleForceRegistry
		{
			MyParticle* particle;
			ForceGenerator* generator;
		};

		std::list<ParticleForceRegistry> Registry;
	};
}


