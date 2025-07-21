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
	segments++;
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
			segments--;
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

	if (curr == nullptr) {
		std::cerr << "Complex Particle Draw nullptr" << std::endl;
		return;
	}

	model->setColor(color);
	while (curr != nullptr) {
		P6::MyParticle* currParticle = curr->data->PhysicsParticle;
		if (!currParticle->IsDestroyed() && curr != nullptr)
		{

			/*POSITION*/
			//model->moveModel(currParticle->Position);
			//std::cout << PhysicsParticle->Position.x << "  " << PhysicsParticle->Position.y << "  " << PhysicsParticle->Position.z << "  " << std::endl;

			model->renderModel();
			if (curr->data->particleType == BasicParticle::PARTICLE) {
				model->drawElemetsTriagnle();
			}
			else if (curr->data->particleType == BasicParticle::LINE) {
				model->drawElementsLine();
			}
		}
		curr = curr->next;
	}
}