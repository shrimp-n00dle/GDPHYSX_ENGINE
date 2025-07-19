#include "Rod.h"

P6::ParticleContact* P6::Rod::GetContact()
{
	float currLen = currentLength();

	/*If the current length is the same as the set length do nothing*/
	if (currLen == length)
	{
		std::cout << "DROPPED LIMIT" << std::endl;
		return nullptr;
	}

	ParticleContact* ret = new ParticleContact();
	ret->particles[0] = particles[0];
	ret->particles[1] = particles[1];

	MyVector dir = particles[1]->Position - particles[0]->Position;

//std::cout << "ROD MENTIONED" << std::endl;

	dir = dir.Direction();


	if (currLen > length)
	{
		ret->contactNormal = dir;

		//depth must always be positive to resolve intepenetration
		ret->depth = currLen - length;
	}

	else
	{
		//invert the direction
		ret->contactNormal = dir.scalarMultiplication(-1.0f);

		//depth must always be positive
		ret->depth = length - currLen;
	}

	ret->restitution = restitution;

	return ret;

}
