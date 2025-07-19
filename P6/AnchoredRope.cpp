#include "AnchoredRope.h"

void P6::AnchoredRope::updateForce(MyParticle* particle, float time)
{
	//Get current position of particle
	MyVector pos = particle->Position;

	MyVector ropePos = pos - anchorPoint;
	float distance = ropePos.Magnitude();

	if (distance > restLength) {
		//Get remainder of the rope
		float displacement = distance - restLength;

		//Get new constrained position and velocity of particle
		MyVector correctionDir = ropePos.Direction();
		float RopeVelocity = particle->Velocity.scalarProduct(correctionDir);
		particle->Velocity = (particle->Velocity - correctionDir.scalarMultiplication(RopeVelocity));

	}

}

