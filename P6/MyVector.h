#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace P6
{
	class MyVector
	{
	public:
		float x, y, z;

		/*Constructor*/
		MyVector() : x(0), y(0), z(0) {};

		/*Component Constructor*/
		MyVector(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}

		/*Explicit type casting*/
		explicit operator glm::vec3() const { return glm::vec3(x, y, z); }

		/*Magnitude*/
		float Magnitude();
		float SqMagnitude();
		float magnitude = 0.0f;

		/*Direction / Normalize*/
		MyVector Direction();

		/*Addition*/
		MyVector operator+ (const MyVector rhs)
		{
			MyVector sum(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);

			return sum;
		}

		//shift ctrl /
		/*MyVector operator+ (float rhs)
		{
			MyVector sum(this->x + rhs, this->y + rhs, this->z + rhs);

			return sum;
		}*/

		/*[+=] Addition*/
		void operator+= (const MyVector rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;

		}

		/*Subtraction*/
		MyVector operator- (const MyVector rhs)
		{
			MyVector difference(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);

			return difference;
		}

		/*Subtraction*/
		MyVector operator-= (const MyVector rhs)
		{
			MyVector difference(this->x -= rhs.x, this->y -= rhs.y, this->z -= rhs.z);

			return difference;
		}

		/*Scalar Multiplication*/
		MyVector scalarMultiplication(const float value);

		/*Component Product*/
		MyVector componentProduct(const MyVector rhs);

		/*Scalar Product / Dot Product*/
		float scalarProduct(const MyVector rhs);

		/*Vector Product / Cross Product*/
		MyVector vectorProduct(const MyVector rhs);

	};

}

