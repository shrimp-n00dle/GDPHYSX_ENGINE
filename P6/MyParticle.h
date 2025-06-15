#pragma once

#include "MyVector.h"

namespace P6
{
	class MyParticle
	{

		/*ATTRIBUTES*/
	public:

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

		MyParticle(std::string name) : Name(name) {};

		MyParticle(const MyParticle& clone)
		{
			mass = clone.mass;
			Position = clone.Position;
			Velocity = clone.Velocity;
			damping = clone.damping;
			Acceleration = clone.Acceleration;
			bBoost = clone.bBoost;
			Name = clone.Name;

		};

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

