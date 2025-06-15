#pragma once

#include <glad/glad.h> /*This helps with simplifying runtime when dealing with pointers*/
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../tiny_obj_loader.h"

#include "../P6/MyVector.h"
#include "Shader.h"

class Model
{
	private:
		/*Attributes*/

		glm::mat4 identity_matrix = glm::mat4(1.0f);

		/*Current positon, transform of the model*/
		glm::mat4 transformation_matrix;

		/*Vertex Array Object* - It stores the format of the vertex data as well as the Buffer Objects (see below) providing the vertex data arrays
 Vertex Buffer Object - store an array of unformatted memory allocated by the OpenGL context (AKA the GPU). These can be used to store vertex data,
 Element Buffer Object - This will reduce the number of vertices by removing redundant ones*/
		GLuint VAO, VBO, EBO;

		std::vector<tinyobj::shape_t> objShapes;

		/*Basic Attributes related to mesh*/
		tinyobj::attrib_t attributes;


		std::vector<GLuint> mesh_indicies;

		Shader shader;

		glm::vec3 modelColor;


		
	public:
		/*Constructor*/
		Model(std::string name, Shader shader);


		/*Linear Transformation*/
		void moveModel(P6::MyVector newPos);
		void scaleModel(P6::MyVector newScale);
		void rotateModel(P6::MyVector newAngle, float theta);


		void renderModel();
		void bind();
		void cleanUp();

		/*Color*/
		void setColor(P6::MyVector newColor);
		
};

