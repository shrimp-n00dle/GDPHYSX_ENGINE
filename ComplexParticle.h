#pragma once
#include "RenderParticle.h"

class Node{
public:
	RenderParticle* particle;
	Node* prev;
	Node* next;


	Node(RenderParticle* particle) : particle(particle) {
		prev = next = nullptr;
	}
};

class ComplexParticle {
	Node* head;
	Node* tail;

public:
	ComplexParticle() {
		head = tail = nullptr;
	}
	
	void PushBack(RenderParticle* particle);

	void DeleteNode(RenderParticle* particle);

	~ComplexParticle();
};