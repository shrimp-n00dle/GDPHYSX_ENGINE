#include "ForceRegistry.h"


void P6::ForceRegistry::Add(MyParticle* particle, ForceGenerator* generator)
{
	ParticleForceRegistry toAdd;
	toAdd.particle = particle;
	toAdd.generator = generator;

	Registry.push_back(toAdd);
}

void P6::ForceRegistry::Clear()
{
	Registry.clear();
}



void P6::ForceRegistry::Remove(MyParticle* particle, ForceGenerator* generator)
{

	Registry.remove_if
	(
		[particle, generator](ParticleForceRegistry reg)
		{
			return reg.particle == particle && reg.generator == generator;
		}
	);
}

void P6::ForceRegistry::updateForces(float time)
{
	for (std::list<ParticleForceRegistry>::iterator i = Registry.begin();
		i != Registry.end(); i++)
	{
		i->generator->updateForce(i->particle, time);
	}
}
