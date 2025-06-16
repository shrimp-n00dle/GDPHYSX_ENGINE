#pragma once
#include "P6/MyVector.h"
#include "P6/EngineParticle.h"
#include "Classes/Model.h"

//using namespace P6;

class RenderParticle
{
public:

	/*ATTRIBUTES*/
	P6::EngineParticle* PhysicsParticle;

	Model* model;

	P6::MyVector Color;

	bool bRecorded = false;

public:
	/*METHODS*/
	RenderParticle(P6::EngineParticle* particle, Model* currModel) : PhysicsParticle(particle), model(currModel) 
	{
		Color = P6::MyVector(1, 1, 1);
	}

	RenderParticle() {};

	RenderParticle(P6::EngineParticle* particle, Model* currModel, P6::MyVector currColor) : PhysicsParticle(particle), model(currModel), Color(currColor) 
	{
		
	};

	void Draw();

	void checkLifespan(float value);

	P6::MyVector randomAcceleration();



};

