#pragma once

#include "P6/ParticleContact.h"
#include <vector>

class ContactResolver
{
public:
	unsigned max_iterations;

	ContactResolver(unsigned _maxIterations) : max_iterations(_maxIterations) {}

	void resolveContacts(std::vector<P6::ParticleContact*> contacts, float time);


protected:
	unsigned current_iterations = 0;

	float leastSSpeed;
};

