#pragma once
#include <list>
#include "MyParticle.h"
#include "ForceRegistry.h"
#include "ParticleContact.h"
#include "GravityForceGenerator.h"
#include "../ContactResolver.h"
#include "ParticleLink.h"
#include <vector>

namespace P6
{
	class PhysicsWorld
	{
	public:

		std::list<ParticleLink*> Links;
		std::vector<ParticleContact*> Contacts;

		void AddContact(MyParticle* p1, MyParticle* p2, float restitution, MyVector contactNormal, float depth);
		ForceRegistry forceRegistry;

		std::list<MyParticle*> particleList;

		void addParticle(MyParticle* particle);

		//Calls the UPDATE OF ALL
		void Update(float time);

	private:
		void updateParticleList();

		//GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, -9.8, 0));
		GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, 0, 0));

	protected:

		ContactResolver contactResolver = ContactResolver(20);

		void generateContacts();

		void getOverlaps();


	};

}


