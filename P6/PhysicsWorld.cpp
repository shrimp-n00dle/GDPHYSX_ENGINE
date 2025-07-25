#include "PhysicsWorld.h"

void P6::PhysicsWorld::addParticle(P6::MyParticle* particle)
{
	particleList.push_back(particle);

	forceRegistry.Add(particle, &Gravity);
}

void P6::PhysicsWorld::AddContact(MyParticle* p1, MyParticle* p2, float restitution, MyVector contactNormal, float depth)
{
	ParticleContact* toAdd = new ParticleContact();

	//Assign needed variables and values
	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = contactNormal;
	toAdd->depth = depth;

	Contacts.push_back(toAdd);
}

void P6::PhysicsWorld::Update(float time)
{
	updateParticleList();

	forceRegistry.updateForces(time);

	for (std::list<MyParticle*>::iterator p = particleList.begin();
		//If its not the end, move to the next particle
		p != particleList.end(); p++)

	{
		/*Call the update*/
		(*p)->updateParticle(time);
	}

	////call generate contacts here
	generateContacts();

	if (Contacts.size() > 0)
	{
		//std::cout << "THERS CONTACT" << std::endl;
		contactResolver.resolveContacts(Contacts, time);
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

void P6::PhysicsWorld::generateContacts()
{
	Contacts.clear();

	getOverlaps();

	for (std::list<ParticleLink*>::iterator i = Links.begin(); i != Links.end(); i++)
	{
		ParticleContact* contact = (*i)->GetContact();

		if (contact != nullptr)
		{
			Contacts.push_back(contact);
		}
	}
}

// Fixed PhysicsWorld.cpp - getOverlaps() function
void P6::PhysicsWorld::getOverlaps()
{
	for (int i = 0; i < particleList.size() - 1; i++)
	{
		std::list<MyParticle*>::iterator a = std::next(particleList.begin(), i);

		for (int h = i + 1; h < particleList.size(); h++)
		{
			std::list<MyParticle*>::iterator b = std::next(particleList.begin(), h);

			MyVector distanceVector = (*a)->Position - (*b)->Position;

			// Calculate actual distance (not squared)
			float distance = distanceVector.Magnitude();
			//float combinedRadius = (*a)->radius + (*b)->radius;
			float combinedRadius = ((*a)->radius + (*b)->radius) * ((*a)->radius + (*b)->radius) * ((*a)->radius + (*b)->radius);

			// DEBUG: Print collision detection info (remove this after testing)
			if (distance < combinedRadius + 0.1f) // Print when close
			{
				std::cout << "Particles close - Distance: " << distance << ", Combined Radius: " << combinedRadius
					<< ", A radius: " << (*a)->radius << ", B radius: " << (*b)->radius << std::endl;
			}

			// Check for collision using actual distances with small tolerance
			// Add small epsilon to prevent floating point precision issues
			if (distance < combinedRadius - 0.001f) // Small tolerance to prevent early collision
			{
				std::cout << "COLLISION DETECTED - Distance: " << distance << ", Combined Radius: " << combinedRadius << std::endl;

				// Calculate penetration depth correctly
				float depth = combinedRadius - distance;

				// Ensure minimum depth
				if (depth < 0.001f) depth = 0.001f;

				// Get collision normal (direction from b to a)
				MyVector contactNormal;
				if (distance > 0.0001f) // Avoid division by zero
				{
					contactNormal = distanceVector.scalarMultiplication(1.0f / distance); // Normalize
				}
				else
				{
					// If particles are exactly on top of each other, use arbitrary normal
					contactNormal = MyVector(1, 0, 0);
				}

				float restitution = fmin((*a)->restitution, (*b)->restitution);

				AddContact(*a, *b, restitution, contactNormal, depth);
			}
		}
	}
}

// Modified PhysicsWorld::Update method - Add this to your PhysicsWorld class
// or create a new method called UpdateWithoutCollisions

void P6::PhysicsWorld::UpdateWithoutCollisions(float time)
{
	updateParticleList();

	forceRegistry.updateForces(time);

	for (std::list<MyParticle*>::iterator p = particleList.begin();
		//If its not the end, move to the next particle
		p != particleList.end(); p++)

	{
		/*Call the update*/
		(*p)->updateParticle(time);
	}

	// Skip collision detection and resolution
	// generateContacts();
	// if (Contacts.size() > 0)
	// {
	//     contactResolver.resolveContacts(Contacts, time);
	// }
}