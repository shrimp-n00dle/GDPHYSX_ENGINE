#include "EngineParticle.h"

void  P6::EngineParticle::updatePosition(float time)
{
	//P2 = P1 + Velocity * time

	MyVector newAccelValue = Acceleration.scalarMultiplication((time * time));


	Position = Position + Velocity.scalarMultiplication(time) + newAccelValue.scalarMultiplication((1.0f / 2.0f));

}

void  P6::EngineParticle::updateVelocity(float time)
{
	Acceleration += accumulatedForce.scalarMultiplication((1.0f / mass));
	/*rate of change in position given the certain time*/

	Velocity = Velocity + (Acceleration.scalarMultiplication(time));

	Velocity = Velocity.scalarMultiplication(powf(damping, time));

}

void P6::EngineParticle::updateParticle(float time)
{
	updatePosition(time);

	updateVelocity(time);

	resetForce();
}

void P6::EngineParticle::Destroy()
{
	bDestroy = true;
}

void P6::EngineParticle::addForce(MyVector force)
{
	accumulatedForce += force;
}

void P6::EngineParticle::resetForce()
{
	accumulatedForce = MyVector(0, 0, 0);

	Acceleration = MyVector(0, 0, 0);
}



