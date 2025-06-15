#include "GravityForceGenerator.h"


void P6::GravityForceGenerator::updateForce(MyParticle* particle, float time)
{
	if (particle->mass <= 0) return;

	MyVector force = Gravity.scalarMultiplication(particle->mass);

	particle->addForce(force);
}