#version 330 core
layout (location = 0) in vec3 aPos;

// Matrices from your C++ code
uniform mat4 transform;   // Model matrix (from your existing code)
uniform mat4 view;        // Camera view matrix (NEW)
uniform mat4 projection;  // Projection matrix (UPDATED)

void main()
{
    // Apply all transformations: Projection * View * Model * Vertex
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
}