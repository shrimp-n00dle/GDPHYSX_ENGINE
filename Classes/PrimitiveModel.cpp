#include "PrimitiveModel.h"
#include <vector>

PrimitiveModel::PrimitiveModel(std::string name, Shader shader)
{
    this->shader = shader;
}


void PrimitiveModel::moveModel(P6::MyVector newPos)
{
    transformation_matrix = glm::translate(identity_matrix, glm::vec3(newPos));
};

void PrimitiveModel::scaleModel(P6::MyVector newScale)
{
    transformation_matrix = glm::scale(transformation_matrix, glm::vec3(newScale));
}

void PrimitiveModel::rotateModel(P6::MyVector newAngle, float theta)
{
    transformation_matrix = glm::rotate(
        transformation_matrix,
        glm::radians(theta),
        glm::normalize(glm::vec3(newAngle)));
}

void PrimitiveModel::addPoint(const std::vector<float>& point)
{
    objShape.push_back(point[0]);
    objShape.push_back(point[1]);
    objShape.push_back(point[2]);
    addIndicies();
}

void PrimitiveModel::addIndicies()
{
    indicies.push_back(indicies.size());
}

void PrimitiveModel::renderModel()
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
}

void PrimitiveModel::bind()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    /*EBO Bind*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * objShapes.size(), &objShapes[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(verticiesSetUp), verticiesSetUp, GL_STATIC_DRAW);

    /*Create Element Array Buffer to store the indicies*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    /*Parameters - Gets the data insied the buffer variable, size of the whole buffer in bytes, index array, rendererer*/
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indicies.size(), &indicies[0], GL_STATIC_DRAW);

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

void PrimitiveModel::STATIC_UpdateBuffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * objShapes.size(), &objShapes[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indicies.size(), &indicies[0], GL_STATIC_DRAW);

}

void PrimitiveModel::cleanUp()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void PrimitiveModel::drawElemetsTriagnle()
{
    //instead of glDrawArrays(GL_TRIANGLES,0,3) you can simplify things by replacing it with the one below
   /*parameters - type of primitive to use, number of vertices, datat type of index*/
    glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
}

void PrimitiveModel::drawElementsLine()
{
    glDrawElements(GL_LINE_STRIP, indicies.size(), GL_UNSIGNED_INT, 0);
}

void PrimitiveModel::setColor(P6::MyVector newColor)
{
    /*How to change color in shaders using code?*/
    modelColor = (glm::vec3)newColor;
}