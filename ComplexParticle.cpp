#include "ComplexParticle.h"

void ComplexParticle::PushBack(RenderParticle* particle)
{
	Node* newNode = new Node(particle);

	if (head == nullptr) {
		head = newNode;
		tail = newNode;
	}
	else {
		tail->next = newNode;
		newNode->prev = tail;
		tail = newNode;
	}
}

void ComplexParticle::DeleteNode(RenderParticle* particle)
{
	Node* curr = head;

	while (curr != nullptr) {
		if (curr->particle == particle) {
			if (curr == head) {
				head = curr->next;
				if (head != nullptr) {
					head->prev = nullptr;
				}
			}
			else if (curr == tail) {
				tail = curr->prev;
				if (tail != nullptr) {
					tail->next = nullptr;
				}
			}
			else {
				curr->prev->next = curr->next;
				curr->next->prev = curr->prev;
			}
			delete curr;
			return;
		}
		curr = curr->next;
	}
}

ComplexParticle::~ComplexParticle()
{
	Node* curr = head;
	while (curr != nullptr) {
		Node* nextNode = curr->next;
		delete curr;
		curr = nextNode;
	}
}
