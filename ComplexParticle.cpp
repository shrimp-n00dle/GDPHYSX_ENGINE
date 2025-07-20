#include "ComplexParticle.h"

void ComplexParticle::PushBack(BasicParticle* particle)
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

void ComplexParticle::DeleteNode(BasicParticle* particle)
{
	Node* curr = head;

	while (curr != nullptr) {
		if (curr->data == particle) {
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

void ComplexParticle::Draw()
{
	//iterate over all basic particles
	Node* curr = head;
	

	if (!curr->data->PhysicsParticle->IsDestroyed())
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
		model->drawElemetsTriagnle();
	}
	
}