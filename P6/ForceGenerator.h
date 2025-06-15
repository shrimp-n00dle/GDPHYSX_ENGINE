#pragma once

#include "MyParticle.h"

/*Base class that computes the froce
to be applied to the target particle*/
namespace P6
{
	class ForceGenerator
	{
	public:

		virtual void updateForce(MyParticle* p, float time)
		{
			p->addForce(MyVector(0, 0, 0));
		}

		float randomForce(int higher, int lower);

		
		//overload
		// void UpdateForce(float time)
		//{
			//do something else
		//};

	};
}

