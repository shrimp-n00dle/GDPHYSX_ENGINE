#pragma once
#include <list>
#include "MyParticle.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"

namespace P6
{
	class PhysicsWorld
	{
		public:

			ForceRegistry forceRegistry;

			std::list<MyParticle*> particleList;

			void addParticle(MyParticle* particle);

			//Calls the UPDATE OF ALL
			void Update(float time);

		private:
			void updateParticleList();

			//GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, -9.8, 0));
			GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, 0, 0));

	};

}


