#pragma once

#include "MyVector.h"

namespace P6
{
	class MyParticle
	{
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
		MyVector accumulatedForce = MyVector(0, 0, 0);

		void updatePosition(float time);
		void updateVelocity(float time);

	public:
		void updateParticle(float time);
		void addForce(MyVector force);
		void resetForce();
		float randomAccel();
		void Destroy();
		bool IsDestroyed() { return bDestroy; }

		// NEWLY ADDED:
		float getInverseMass() const {
			return (mass <= 0.0f) ? 0.0f : (1.0f / mass);
		}

		void setInverseMass(float invMass) {
			if (invMass == 0.0f)
				mass = 0.0f;
			else
				mass = 1.0f / invMass;
		}
	};
}
