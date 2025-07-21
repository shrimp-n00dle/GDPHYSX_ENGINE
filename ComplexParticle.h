#pragma once
#include "P6/MyVector.h"
#include "P6/MyParticle.h"
#include "Classes/PrimitiveModel.h"
class BasicParticle {
public:
	enum Type { PARTICLE = 0, LINE = 1, LINE_END = 2 };

	P6::MyParticle* PhysicsParticle;

	std::string Name;

	Type particleType;

public:
	/*METHODS*/
	BasicParticle(std::string name, P6::MyParticle* particle) : Name(name), PhysicsParticle(particle), particleType(PARTICLE) {}

	BasicParticle(std::string name, P6::MyParticle* particle, enum Type particleType) : Name(name), PhysicsParticle(particle), particleType(particleType) {}
};


class Node{
public:
	BasicParticle* data;
	Node* prev;
	Node* next;


	Node(BasicParticle* particle) : data(particle) {
		prev = next = nullptr;
	}
};

class ComplexParticle {
	Node* head;
	Node* tail;

	std::string Name;
	PrimitiveModel* model;
	P6::MyVector color;
	int segments;

public:
	ComplexParticle(std::string name, PrimitiveModel* currModel) :  Name(name), model(currModel) {
		head = tail = nullptr;
		color = P6::MyVector(1, 1, 1);
		segments = 0;
	}

	ComplexParticle(std::string name, PrimitiveModel* currModel, P6::MyVector currColor) : Name(name), model(currModel), color(currColor)
	{
		head = tail = nullptr;
		segments = 0;
	}
	
	void PushBack(BasicParticle* particle);

	void DeleteNode(BasicParticle* particle);

	void Draw();

	~ComplexParticle();
};