#include "Model.h"


Model::Model(std::string name, Shader shader) 
{
    this->shader = shader;

    std::string path = name;
    std::vector<tinyobj::material_t> objMeshShapes;

    std::string warning, error;

    /*Load Mesh*/
    bool success = tinyobj::LoadObj(&attributes, &objShapes, &objMeshShapes, &warning, &error, path.c_str());
}

void Model::moveModel(P6::MyVector newPos)
{
	transformation_matrix = glm::translate(identity_matrix, glm::vec3(newPos));
};

void Model::scaleModel(P6::MyVector newScale)
{
	transformation_matrix = glm::scale( transformation_matrix, glm::vec3(newScale));
}

void Model::rotateModel(P6::MyVector newAngle, float theta)
{
	transformation_matrix = glm::rotate(
     transformation_matrix,
	 glm::radians(theta),
	 glm::normalize(glm::vec3(newAngle)));
}

void Model::renderModel()
{
    //Setting the projection 
    unsigned int projLoc = glGetUniformLocation(shader.shaderProg, "projection");
    glUniformMatrix4fv(projLoc, //Address of the variable
                       1, //How many value are we modifying
                        GL_FALSE, 
                        glm::value_ptr(transformation_matrix)); //Projection Matrix

    //Get the variable named transform from one of the shaders
   //attached to the shaderProg
    unsigned int transformLoc = glGetUniformLocation(shader.shaderProg, "transform");

    //assign the matrix
   glUniformMatrix4fv(transformLoc, //Address to the transform variable
       1, //How many matrices to assign
        GL_FALSE, //Transpose
        glm::value_ptr(transformation_matrix)); // pointer to the matrix


   /*COLOR CHANGE*/
   int color = glGetUniformLocation(shader.shaderProg, "currColor");
   glUniform3f(color, modelColor.x, modelColor.y, modelColor.z);


   /*Triangle Rendering*/
      //Call binder for renderer
   glBindVertexArray(VAO);

   //instead of glDrawArrays(GL_TRIANGLES,0,3) you can simplify things by replacing it with the one below
   /*parameters - type of primitive to use, number of vertices, datat type of index*/
   glDrawElements(GL_TRIANGLES, mesh_indicies.size(), GL_UNSIGNED_INT, 0);
}

void Model::bind()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    /*EBO Bind*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    for (int i = 0; i < objShapes[0].mesh.indices.size(); i++)
    {
        mesh_indicies.push_back(objShapes[0].mesh.indices[i].vertex_index);
    }


    glUseProgram(shader.shaderProg);


    /*BINDING*/
  /*Bind the VAO so all other funtions after this will follow the VAO*/
    glBindVertexArray(VAO);

    /*Assigning the Array Buffers to store the added positions*/
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*Converting VAO into bytes
    Paramater names - What data is the buffer located
    The size of the whole buffer
    the vertext array
    renderer*/

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * attributes.vertices.size(), &attributes.vertices[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(verticiesSetUp), verticiesSetUp, GL_STATIC_DRAW);

    /*Create Element Array Buffer to store the indicies*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    /*Parameters - Gets the data insied the buffer variable, size of the whole buffer in bytes, index array, rendererer*/
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh_indicies.size(), mesh_indicies.data(), GL_STATIC_DRAW);

    /*How can the VAO interpte VBO?
    Parameters - VAO Id number, 3 bc XYZ, what datat do u want it to return, not sure, the size of the vertex data
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    /*VAO to use the data above*/
    glEnableVertexAttribArray(0);

    /*Binding the buffer and the Id number of VAO*/
    /*gl BindBuffer is acalled twice bc the first glBindBuffer has been formatted and overwritten by glBufferData/glVertexAttribPointer*/
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*Overwritten by the  glBufferData/glVertexAttribPointer*/
    glBindVertexArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::cleanUp()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Model::setColor(P6::MyVector newColor)
{
    /*How to change color in shaders using code?*/
    modelColor = (glm::vec3)newColor;
}