#include "ParticleContact.h"

float P6::ParticleContact::GetSeparatingSpeed()
{
	MyVector velocity = particles[0]->Velocity;

	if (particles[1]) velocity -= particles[1]->Velocity;
	return velocity.scalarProduct(contactNormal);

}

void P6::ParticleContact::resolve(float time)
{
	resolveVelocity(time);

	resolveInterpenet(time);
}

void P6::ParticleContact::resolveVelocity(float time)
{
	float separatingSpeed = GetSeparatingSpeed();

	//if greater than 0, it is already moving away
	//so we ignore it
	if (separatingSpeed > 0) 
	{
		return;
	}

	/*std::cout << "COLLISION!" << std::endl;
	std::cout << separatingSpeed << std::endl;*/

	float newSS = -restitution * separatingSpeed;

	//Magnitude of delta velocity
	float deltaSpeed = newSS - separatingSpeed;

	float totalMass = (float)1 / particles[0]->mass;

	if (particles[1]) totalMass += (float)1 / particles[1]->mass;

	//if mass == 0 and negative invalid collision if 0 or less
	if (totalMass <= 0) return;

	//std::cout << "COLLISION IS VALID!" << std::endl;

	////Magnitude of impulse vector
	float impulseMag = deltaSpeed / totalMass;
	MyVector impulse = contactNormal.scalarMultiplication(impulseMag); 

	MyVector v_a = impulse.scalarMultiplication((float)1 / particles[0]->mass);
	particles[0]->Velocity = particles[0]->Velocity + v_a;

	if (particles[1])
	{
		MyVector v_b = impulse.scalarMultiplication((float)-1 / particles[1]->mass);
		particles[1]->Velocity = particles[1]->Velocity + v_b;
	}

}

void P6::ParticleContact::resolveInterpenet(float time)
{
	//if they are not overlapping - Skip
	if (depth <= 0) return;

	//get total mass of the collision
	float totalMass = (float)1 / particles[0]->mass;
	if (particles[1]) totalMass += (float)1 / particles[1]->mass;

	//Invalid collision if total mass is 0 or less
	if (totalMass <= 0) return;

	//How many units to move per total mass
	float totalMovebyMass = depth / totalMass;

	//Get the vector of the total movement involved
	MyVector moveByMass = contactNormal.scalarMultiplication(totalMovebyMass);

	//Get the change in position for A
	MyVector P_a = moveByMass.scalarMultiplication((float)1 / particles[0]->mass);

	//translate a
	particles[0]->Position += P_a;

	if (particles[1])
	{
		//Get the change in position for B in the opp direction
		MyVector P_b = moveByMass.scalarMultiplication(-(float)1 / particles[1]->mass);

		//Translate B
		particles[1]->Position += P_b;
	}

	//After solving assume the depth is now 0
	depth = 0;
}