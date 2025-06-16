#pragma once

#include "MyVector.h"

namespace P6
{
	class EngineParticle
	{

		/*ATTRIBUTES*/
	public:

		float lifespan = 0;

		MyVector Acceleration;

		MyVector Position;

		MyVector Velocity;

		float damping = 0.9f;

		bool bSecond = false;

		/*OPTIONAL*/
		float radius = 0.0f;
		float mass = 0.1f;
		MyVector accumulatedForce;


	protected:
		bool bDestroy = false;


		/*METHODS*/
	protected:

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

