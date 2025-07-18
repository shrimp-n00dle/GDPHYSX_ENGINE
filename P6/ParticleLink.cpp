#include "ParticleLink.h"


float P6::ParticleLink::currentLength()
{
	MyVector ret = particles[0]->Position - particles[1]->Position;

	return ret.Magnitude();
}