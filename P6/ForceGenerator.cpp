#include "ForceGenerator.h"

float P6::ForceGenerator::randomForce(int higher, int lower)
{
	return rand() % higher + lower;
}
