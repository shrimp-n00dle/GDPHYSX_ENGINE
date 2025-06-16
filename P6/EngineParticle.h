#pragma once

#include "MyVector.h"

/*Particle class made for Phase 1*/
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

		/*A boolean value holding if a second has passed with the particle on screen, if true, 
		the lifespan will be deducted by 1 and will be false again afterwards*/
		bool bSecond = false;

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

	public:
		void Destroy();

		bool IsDestroyed() { return bDestroy; }
	};
}

