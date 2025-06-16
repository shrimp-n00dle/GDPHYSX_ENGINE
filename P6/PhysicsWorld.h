#pragma once
#include <list>
#include "EngineParticle.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"

namespace P6
{
	class PhysicsWorld
	{
		public:

			ForceRegistry forceRegistry;

			std::list<EngineParticle*> particleList;

			void addParticle(EngineParticle* particle);

			//Calls the UPDATE OF ALL
			void Update(float time);

		private:
			void updateParticleList();

			//GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, -9.8f, 0));
			GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, 0, 0));

	};

}


