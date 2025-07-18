#define TINYOBJLOADER_IMPLEMENTATION

/*Shader implementation*/
#include <string>
#include <iostream>

// GLM includes (make sure these are included)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*P6 and Custom Classes inclusion*/
#include "P6/MyVector.h"
#include "P6/MyParticle.h"
#include "P6/PhysicsWorld.h"
#include "P6/DragForceGenerator.h"

#include "RenderParticle.h"
#include "Classes/Model.h"
#include "Classes/Shader.h"
#include "ContactResolver.h"

//Import the libraries
#include <chrono>
using namespace std::chrono_literals;
//This is going to be our time in between "frames"
constexpr std::chrono::nanoseconds timestep(30ms);

// Camera variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 2.0f;

// Projection variables
bool isPerspective = true; // Start with perspective

// Pause variable
bool isPaused = false;

// Input callback function
void processInput(GLFWwindow* window, float deltaTime)
{
    // WASD Camera movement (orbiting around center point)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        // Move camera up
        cameraPos.y += cameraSpeed * deltaTime;
        std::cout << "W pressed - Camera Y: " << cameraPos.y << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        // Move camera down
        cameraPos.y -= cameraSpeed * deltaTime;
        std::cout << "S pressed - Camera Y: " << cameraPos.y << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        // Rotate camera left around target
        float angle = cameraSpeed * deltaTime;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 offset = cameraPos - cameraTarget;
        glm::vec4 newOffset = rotation * glm::vec4(offset, 1.0f);
        cameraPos = cameraTarget + glm::vec3(newOffset);
        std::cout << "A pressed - Camera rotated left" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        // Rotate camera right around target
        float angle = -cameraSpeed * deltaTime;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 offset = cameraPos - cameraTarget;
        glm::vec4 newOffset = rotation * glm::vec4(offset, 1.0f);
        cameraPos = cameraTarget + glm::vec3(newOffset);
        std::cout << "D pressed - Camera rotated right" << std::endl;
    }

    // --- Projection switching (1: Orthographic, 2: Perspective) ---
    static bool key1Last = false;
    static bool key2Last = false;

    bool key1Now = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
    bool key2Now = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;

    if (key1Now && !key1Last)
    {
        isPerspective = false; // Key 1 = Orthographic
        std::cout << "Switched to ORTHOGRAPHIC projection" << std::endl;
    }
    if (key2Now && !key2Last)
    {
        isPerspective = true; // Key 2 = Perspective
        std::cout << "Switched to PERSPECTIVE projection" << std::endl;
    }
    key1Last = key1Now;
    key2Last = key2Now;

    // Spacebar pause toggle (single press detection)
    static bool spacePressed = false;
    bool currentSpaceState = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
    if (currentSpaceState && !spacePressed)
    {
        isPaused = !isPaused;
        std::cout << (isPaused ? "Simulation PAUSED" : "Simulation RESUMED") << std::endl;
    }
    spacePressed = currentSpaceState;

    // ESC to close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "Group 8 / Bubble Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*calling the initializing of glad*/
    gladLoadGL();

    glViewport(0, //Min X
        0, //Min Y
        800, //Width
        800); //Height

    // Enable depth testing for 3D
    glEnable(GL_DEPTH_TEST);

    /*Shader*/
    Shader modelShader = Shader();

    /*Model*/
    Model model = Model("3D/sphere.obj", modelShader);

    /*Model Binding*/
    model.bind();

    /*PHYSICS WORLD IMPLEMENTATION*/
    P6::PhysicsWorld pWorld = P6::PhysicsWorld();
    /*RENDER PARTICLE IMPLEMENTATION*/
    std::list<RenderParticle*> rParticleList;

    /*PARTICLE IMPLEMETATION - BUNGEE*/
    P6::MyParticle pBungeeAnchor = P6::MyParticle();
    pBungeeAnchor.Position = P6::MyVector(-0.2, 0.5, 0);
    pBungeeAnchor.mass = 10;
    pBungeeAnchor.restitution = 1;
    pBungeeAnchor.radius = 0.2f;
    pBungeeAnchor.Velocity = P6::MyVector(0, 0, 0);

    pWorld.addParticle(&pBungeeAnchor);
    RenderParticle rBunA = RenderParticle("Pholder", &pBungeeAnchor, &model, P6::MyVector(0.0f, 0.0f, 1.0f));
    rBunA.model->scaleModel(P6::MyVector(pBungeeAnchor.radius, pBungeeAnchor.radius, pBungeeAnchor.radius));
    rParticleList.push_back(&rBunA);

    P6::MyParticle particle = P6::MyParticle();
    particle.Position = P6::MyVector(-0.2, -0.5, 0);
    particle.mass = 15;
    particle.radius = 0.2f;
    particle.restitution = 1;
    particle.addForce(P6::MyVector(0, 0.1, 0).scalarMultiplication(1.0));
    particle.Velocity = P6::MyVector(0, 0.1, 0);
    pWorld.addParticle(&particle);

    // P6::Bungee pBungee = P6::Bungee(&pBungeeAnchor, 0.5, 0.25);
    // pWorld.forceRegistry.Add(&particle, &pBungee);

    RenderParticle rParticle = RenderParticle("P1", &particle, &model, P6::MyVector(4.0f, 0.0f, 0.0f));
    rParticle.model->scaleModel(P6::MyVector(particle.radius, particle.radius, particle.radius));
    rParticleList.push_back(&rParticle);

    /*TIME IMPLEMENTATION*/
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);
    std::chrono::milliseconds timer(0);

    // Delta time for smooth camera movement
    auto lastFrameTime = clock::now();

    // Print initial controls
    std::cout << "=== CONTROLS ===" << std::endl;
    std::cout << "WASD: Camera movement" << std::endl;
    std::cout << "Q/E: Decrease/Increase spawn rate" << std::endl;
    std::cout << "1: Orthographic projection" << std::endl;
    std::cout << "2: Perspective projection" << std::endl;
    std::cout << "SPACE: Pause/Resume simulation" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    std::cout << "================" << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /*TIME IMPLEMENTAITION*/
        curr_time = clock::now();

        // Calculate delta time for camera movement
        auto deltaTimeDuration = curr_time - lastFrameTime;
        float deltaTime = std::chrono::duration<float>(deltaTimeDuration).count();
        lastFrameTime = curr_time;

        // Process input - THIS IS CRITICAL FOR INPUT TO WORK
        processInput(window, deltaTime);

        //Duration checker
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);
        auto timeAdd = std::chrono::duration_cast<std::chrono::milliseconds> (curr_time - prev_time);

        if (!isPaused) // Only update timer and physics when not paused
        {
            timer += timeAdd;
        }

        prev_time = curr_time;

        //add dur to last iteration to the time since our last frame
        if (!isPaused) // Only update physics when not paused
        {
            curr_ns += dur;
            if (curr_ns >= timestep)
            {
                //ms converstion
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
                //Reset
                curr_ns -= curr_ns;
                pWorld.Update((float)ms.count() / 1000);
            }
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ===== CRITICAL FIX: ACTIVATE SHADER AND SET CAMERA MATRICES =====
        glUseProgram(modelShader.shaderProg);  // Use your existing shader program

        // Create projection matrix based on current mode
        glm::mat4 projection;
        if (isPerspective)
        {
            projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
        }
        else
        {
            projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -10.0f, 10.0f);
        }

        // Create view matrix based on camera position
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

        // Send projection matrix to shader
        unsigned int projLoc = glGetUniformLocation(modelShader.shaderProg, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Send view matrix to shader
        unsigned int viewLoc = glGetUniformLocation(modelShader.shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Draw all active particles
        for (std::list<RenderParticle*>::iterator i = rParticleList.begin();
            i != rParticleList.end(); i++)
        {

            /*Draw the results*/
            (*i)->Draw();
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents(); // <-- This is required for input to work!
    }

    /*Clean the vertex annd buffers*/
    model.cleanUp();

    glfwTerminate();
    return 0;
}