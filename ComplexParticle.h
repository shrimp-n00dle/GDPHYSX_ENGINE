#pragma once
#include "RenderParticle.h"
#include "P6/MyVector.h"
#include "P6/MyParticle.h"
class BasicParticle {
public:
	enum Type { PARTICLE = 0, LINE_ANCHOR = 1, LINE_END = 2 };

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
	Model* model;
	P6::MyVector color;

public:
	ComplexParticle(std::string name, Model* currModel) :  Name(name), model(currModel) {
		head = tail = nullptr;
		color = P6::MyVector(1, 1, 1);
	}

	ComplexParticle(std::string name, Model* currModel, P6::MyVector currColor) : Name(name), model(currModel), color(currColor) 
	{
		head = tail = nullptr;
	}
	
	void PushBack(BasicParticle* particle);

	void DeleteNode(BasicParticle* particle);

	void Draw();

	~ComplexParticle();
};