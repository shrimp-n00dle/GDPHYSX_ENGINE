#include "PhysicsWorld.h"

void P6::PhysicsWorld::addParticle(P6::EngineParticle* particle)
{
	particleList.push_back(particle);
	
	forceRegistry.Add(particle, &Gravity);
}

void P6::PhysicsWorld::Update(float time)
{
	updateParticleList();

	forceRegistry.updateForces(time);

	for (std::list<EngineParticle*>::iterator p = particleList.begin();
		//If its not the end, move to the next particle
		p != particleList.end(); p++)	
	{

		/*Call the update*/
		(*p)->updateParticle(time);
	}
}

void P6::PhysicsWorld::updateParticleList()
{
	particleList.remove_if( 
		
		[](EngineParticle* p)
			{
			return p->IsDestroyed();
			}
	);

}
