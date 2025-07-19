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
		//std::cout << PhysicsParticle->Position.x << "  " << PhysicsParticle->Position.y << "  " << PhysicsParticle->Position.z << "  " << std::endl;

		/*SCALE*/
		model->scaleModel(P6::MyVector(PhysicsParticle->radius, PhysicsParticle->radius, PhysicsParticle->radius));

		model->renderModel();
	}
}

int RenderParticle::recordTime(float value, int rank)
{
	if (!bRecorded)
	{
		rank++;
		std::cout << Name << "Rank: " << rank << " ,total time: " << value << "seconds" << std::endl;

		bRecorded = true;


	}

	return rank;
}

void RenderParticle::DrawLine(std::string name, P6::MyParticle* anchor, P6::MyParticle* particle)
{

}

