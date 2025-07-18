#include "MyVector.h"

float P6::MyVector::Magnitude()
{
	magnitude = (sqrtf(x * x) + sqrtf(y * y) + sqrt(z * z));
	return magnitude;
}

float P6::MyVector::SqMagnitude()
{
	magnitude = ((x * x) + (y * y) + (z * z));
	return magnitude;
}

P6::MyVector P6::MyVector::Direction()
{
	return MyVector(x / magnitude, y / magnitude, z / magnitude);
}

P6::MyVector P6::MyVector::scalarMultiplication(const float value)
{
	MyVector product(this->x * value, this->y * value, this->z * value);

	return product;
}

P6::MyVector P6::MyVector::componentProduct(const MyVector rhs)
{
	MyVector product(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z);

	return product;
}


float P6::MyVector::scalarProduct(const MyVector rhs)
{
	//float product = (this->x * rhs.x) + (this->y * rhs.y) + (this->z * rhs.z);

	return (this->x * rhs.x) + (this->y * rhs.y) + (this->z * rhs.z);
	//product;
}

P6::MyVector P6::MyVector::vectorProduct(const MyVector rhs)
{
	MyVector product(
		(this->y * rhs.z) - (this->z * rhs.y),
		(this->z * rhs.x) - (this->x * rhs.z),
		(this->x * rhs.y) - (this->y * rhs.x)
	);

	return product;
}
