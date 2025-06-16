#include "DragForceGenerator.h"

void P6::DragForceGenerator::updateForce(EngineParticle* particle, float time)
{
	MyVector force = MyVector(0, 0, 0);
	
	MyVector currV = particle->Velocity;


	float mag = currV.Magnitude();
	if (mag <= 0) return;


	float dragF = (k1 * mag) + (k2 * mag);
	MyVector dir = currV.Direction();

	/*Apply the force to the opp direction*/
	particle->addForce(dir.scalarMultiplication(-dragF));
}
