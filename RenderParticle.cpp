#include "RenderParticle.h"

void RenderParticle::Draw()
{
	if (!PhysicsParticle->IsDestroyed())
	{
		/*COLOR*/
		model->setColor(Color);

		/*POSITION*/
		model->moveModel(PhysicsParticle->Position);

		/*SCALE*/
		model->scaleModel(P6::MyVector(PhysicsParticle->radius, PhysicsParticle->radius, PhysicsParticle->radius));

		model->renderModel();
	}
}

void RenderParticle::checkLifespan(float value)
{
	//the float value holds the time passing 

	//if the time is more than a second, deduct 1 by the lifepsan of the particle, resetting of timer is called in Main.cpp
	if (value >= 1.0f)
	{
		PhysicsParticle->lifespan -= value;
		PhysicsParticle->bSecond = true;
	}

}


