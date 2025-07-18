#pragma once
#include "P6/MyVector.h"
#include "P6/MyParticle.h"
#include "Classes/Model.h"

//using namespace P6;

class RenderParticle
{
public:

	/*ATTRIBUTES*/
	P6::MyParticle* PhysicsParticle;

	std::string Name;

	Model* model;

	P6::MyVector Color;

	bool bRecorded = false;

public:
	/*METHODS*/
	RenderParticle(std::string name, P6::MyParticle* particle, Model* currModel) : Name(name), PhysicsParticle(particle), model(currModel)
	{
		Color = P6::MyVector(1, 1, 1);
	}

	RenderParticle(std::string name, P6::MyParticle* particle, Model* currModel, P6::MyVector currColor) : Name(name), PhysicsParticle(particle), model(currModel), Color(currColor) {};

	void Draw();

	int recordTime(float value, int rank);

	P6::MyVector randomAcceleration();



};

