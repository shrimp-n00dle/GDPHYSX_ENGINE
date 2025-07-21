#pragma once

#include <glad/glad.h> /*This helps with simplifying runtime when dealing with pointers*/
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../tiny_obj_loader.h"

#include "../P6/MyVector.h"
#include "Shader.h"

class PrimitiveModel
{
private:
	/*Attributes*/

	glm::mat4 identity_matrix = glm::mat4(1.0f);

	/*Current positon, transform of the model*/
	glm::mat4 transformation_matrix;

	GLuint VAO, VBO, EBO;

	std::vector<float> objShape;
	std::vector<int> indicies;

	Shader shader;

	glm::vec3 modelColor;

public:
	/*Constructor*/
	PrimitiveModel(std::string name, Shader shader);


	/*Linear Transformation*/
	void moveModel(P6::MyVector newPos);
	void scaleModel(P6::MyVector newScale);
	void rotateModel(P6::MyVector newAngle, float theta);

	void addPoint(const std::vector<float>& data);
	void addIndicies();
	void renderModel();
	void bind();
	void STATIC_UpdateBuffers();
	void cleanUp();

	void drawElemetsTriagnle();
	void drawElementsLine();

	/*Color*/
	void setColor(P6::MyVector newColor);

};

