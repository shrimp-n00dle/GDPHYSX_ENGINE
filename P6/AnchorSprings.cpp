#include "AnchorSprings.h"

void P6::AnchorSprings::updateForce(MyParticle* particle, float time)
{
	//get current position of the particle
	MyVector pos = particle->Position;

	//create a vector Pos -> anchorPoint
	MyVector force = pos - anchorPoint;

	//get the magnitude of the vector above
	float mag = force.Magnitude();

	//f = -k * (|lcurr - lrest|)
	float springForce = -springConstant * abs(mag - restLength);

	//get the direction from the particle to the anchor point
	force = force.Direction();

	//apply force
	force = force.scalarMultiplication(springForce);

	//add force
	particle->addForce(force);
}
