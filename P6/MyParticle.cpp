#include "MyParticle.h"

void  P6::MyParticle::updatePosition(float time)
{
	//P2 = P1 + Velocity * time

	MyVector newAccelValue = Acceleration.scalarMultiplication((time * time));


	Position = Position + Velocity.scalarMultiplication(time) + newAccelValue.scalarMultiplication((1.0f / 2.0f));

}

void  P6::MyParticle::updateVelocity(float time)
{
	Acceleration += accumulatedForce.scalarMultiplication((1.0f / mass));
	/*rate of change in position given the certain time*/


		//V = (P2 - P1) /  time
	Velocity = Velocity + (Acceleration.scalarMultiplication(time));

	Velocity = Velocity.scalarMultiplication(powf(damping, time));

	//P2 = P1 + Vt

	//Position / time;

}

void P6::MyParticle::updateParticle(float time)
{
	updatePosition(time);

	updateVelocity(time);

	resetForce();
}

void P6::MyParticle::Destroy()
{
	bDestroy = true;
}

void P6::MyParticle::addForce(MyVector force)
{
	accumulatedForce += force;
}

void P6::MyParticle::resetForce()
{
	accumulatedForce = MyVector(0, 0, 0);

	Acceleration = MyVector(0, 0, 0);
}

float P6::MyParticle::randomAccel()
{
	if (!bBoost)
	{
		return rand() % 8 + 1.1f;
		bBoost = true;
	}
	else return 0.0f;

}



