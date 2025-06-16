#include "RenderParticle.h"

void RenderParticle::Draw()
{
	if (!PhysicsParticle->IsDestroyed())
	{
		/*COLOR*/
		//model or particle? MODEL MUNA
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
	if (value >= 1.0f)
	{
		PhysicsParticle->lifespan -= value;
	}

	if (PhysicsParticle->lifespan <= 0.0f) PhysicsParticle->Destroy();


}


