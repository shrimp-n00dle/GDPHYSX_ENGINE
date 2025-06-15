#pragma once

#include <glad/glad.h> /*This helps with simplifying runtime when dealing with pointers*/
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
	public:
		GLuint shaderProg;
		/**/
		Shader();
};

