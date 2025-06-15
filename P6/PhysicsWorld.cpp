#include "PhysicsWorld.h"

void P6::PhysicsWorld::addParticle(P6::MyParticle* particle)
{
	particleList.push_back(particle);
	
	forceRegistry.Add(particle, &Gravity);
}

void P6::PhysicsWorld::Update(float time)
{
	updateParticleList();

	forceRegistry.updateForces(time);

	for (std::list<MyParticle*>::iterator p = particleList.begin();
		//If its not the end, move to the next particle
		p != particleList.end(); p++)
	
	{
		//std::cout << "COUNTER: " << *p << std::endl;
		/*Call the update*/
		(*p)->updateParticle(time);
	}
}

void P6::PhysicsWorld::updateParticleList()
{
	particleList.remove_if( 
		
		[](MyParticle* p)
			{
			return p->IsDestroyed();
			}
	);

}
