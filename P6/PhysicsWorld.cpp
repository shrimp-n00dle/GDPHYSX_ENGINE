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

void P6::PhysicsWorld::getOverlaps()
{
	for (int i = 0; i < particleList.size() - 1; i++)
	{
		std::list<MyParticle*>::iterator a = std::next(particleList.begin(), i);

		for (int h = i + 1; h < particleList.size(); h++)
		{
			std::list<MyParticle*>::iterator b = std::next(particleList.begin(), h);

			MyVector mag2Vector = (*a)->Position - (*b)->Position;

			double mag2 = mag2Vector.SqMagnitude();
			double rad = (*a)->radius + (*b)->radius;
			double rad2 = rad * rad;

			if (mag2 <= rad2)
			{
				std::cout << "COLLISION DETECTED" << std::endl;
				MyVector dir = mag2Vector.Direction();
				double r = rad2 - mag2;
				double depth = sqrt(r);

				double restitution = fmin((*a)->restitution, (*b)->restitution);

				AddContact(*a, *b, restitution, dir, depth);
			}
		}
	}
}
