#pragma once

#include "MyVector.h"

namespace P6
{
	class MyParticle
	{

		/*ATTRIBUTES*/
	public:

		float mass = 0.1f;
		float lifespan = 0;

		float radius = 1.0f;

		float restitution = 1.0f;

		MyVector Position;

		MyVector Velocity;

		float damping = 0.9f;

		MyVector Acceleration;

		bool bBoost = false;

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

