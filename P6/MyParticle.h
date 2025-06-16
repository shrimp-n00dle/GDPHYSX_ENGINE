#pragma once

#include "MyVector.h"

/*The Particle class used for Phase 1 is the EngineParticle class*/
namespace P6
{
	class MyParticle
	{

	/*ATTRIBUTES*/
	public:

		float lifespan = 0;

		float mass = 0;

		MyVector Position;

		MyVector Velocity;

		float damping = 0.9f;

		MyVector Acceleration;

		bool bBoost = false;

		std::string Name;

	protected:
		bool bDestroy = false;


		/*METHODS*/
	protected:
		//force gathered before the FIXED update
		MyVector accumulatedForce = MyVector(0, 0, 0);

		void updatePosition(float time);

		void updateVelocity(float time);

	public:

		//Updates the particle and the methods above for EACH PARTICLE
		void updateParticle(float time);

		void addForce(MyVector force);
		void resetForce();

		float randomAccel();

	public:
		void Destroy();

		bool IsDestroyed() { return bDestroy; }
	};
}

