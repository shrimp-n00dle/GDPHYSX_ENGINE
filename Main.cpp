#define TINYOBJLOADER_IMPLEMENTATION

/*Shader implementation*/
#include <string>
#include <iostream>

/*P6 and Custom Classes inclusion*/
#include "P6/MyVector.h"
#include "P6/EngineParticle.h"
#include "P6/PhysicsWorld.h"
#include "P6/DragForceGenerator.h"

#include "RenderParticle.h"
#include "Classes/Model.h"
#include "Classes/Shader.h"

//Import the libraries
#include <chrono>
using namespace std::chrono_literals;
//This is going to be our time in between "frames"
constexpr std::chrono::nanoseconds timestep(16ms);

// Camera variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 2.0f;

// Projection variables
bool isPerspective = true; // Start with perspective
bool key1Pressed = false;
bool key2Pressed = false;

// Pause variable
bool isPaused = false;
bool spacePressed = false; // To handle single press detection

// Input callback function
void processInput(GLFWwindow* window, float deltaTime)
{
    // WASD Camera movement (orbiting around center)
    float radius = glm::length(cameraPos - cameraTarget);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        // Move camera up
        cameraPos.y += cameraSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        // Move camera down
        cameraPos.y -= cameraSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        // Rotate camera left around target
        float angle = cameraSpeed * deltaTime;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec4 newPos = rotation * glm::vec4(cameraPos - cameraTarget, 1.0f);
        cameraPos = glm::vec3(newPos) + cameraTarget;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        // Rotate camera right around target
        float angle = -cameraSpeed * deltaTime;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec4 newPos = rotation * glm::vec4(cameraPos - cameraTarget, 1.0f);
        cameraPos = glm::vec3(newPos) + cameraTarget;
    }

    // Spacebar pause toggle (single press detection)
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
    window = glfwCreateWindow(800, 800, "Group 8 - Phase 1", NULL, NULL);
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

    // Initial projection matrix (will be updated in main loop)
    glm::mat4 projection;

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


    for (int i = 0; i <= 100; i++)
    {
        /*RANDOM GENERATOR*/

        //color

        float color = rand() % 10;
        float color2 = rand() % 10;
        float color3 = rand() % 10;

        float veloValue = rand() % 1 + -1.0f;
        float veloValue2 = rand() % 8;

        float accelValue = rand() % 8 + 1.0f;
        float accelValue2 = rand() % 8;

        //std::cout << "Lifepsan is " << p->lifespan << std::endl;



        P6::EngineParticle* p = new P6::EngineParticle();
        p->Velocity = P6::MyVector(veloValue, veloValue2, 0);
        p->Position = P6::MyVector(0, -0.7f, 0);
        // p->Position = P6::MyVector(0, -0.1f, 0);
        p->Acceleration = P6::MyVector(accelValue, accelValue2, 0);


        P6::ForceGenerator* f = new P6::ForceGenerator;

        p->addForce(P6::MyVector(0, 3, 0));

        //radius
        p->radius = (float)(rand() % 1 + 0.20f);// 0.10f;

        //lifespan
        p->lifespan = 1.0f;//rand() % 1 + 1.0f;


        f->updateForce(p, 0.1f);
        pWorld.forceRegistry.Add(p, f);
        pWorld.addParticle(p);
        RenderParticle* rp = new RenderParticle(p, &model, P6::MyVector(color, color2, 0.0f));
        rParticleList.push_back(rp);
    }

    /*TIME IMPLEMENTATION*/
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);
    std::chrono::milliseconds timer(0);

    // Delta time for smooth camera movement
    auto lastFrameTime = clock::now();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /*TIME IMPLEMENTAITION*/
        curr_time = clock::now();

        // Calculate delta time for camera movement
        auto deltaTimeDuration = curr_time - lastFrameTime;
        float deltaTime = std::chrono::duration<float>(deltaTimeDuration).count();
        lastFrameTime = curr_time;

        // Process input
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

        // Update projection matrix based on current mode
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

        // You'll need to pass these matrices to your shader
        // This depends on how your Shader class is implemented
        // Example (adjust according to your shader implementation):
        // modelShader.use();
        // modelShader.setMat4("projection", projection);
        // modelShader.setMat4("view", view);

        if (!isPaused) // Only print timer when not paused
        {
            std::cout << (float)timer.count() / 1000 << std::endl;
        }

        for (std::list<RenderParticle*>::iterator i = rParticleList.begin();
            i != rParticleList.end(); i++)
        {
            /*Check lifespan first*/
            if ((*i)->PhysicsParticle->lifespan > 0.0f && !isPaused) // Only update lifespan when not paused
            {
                (*i)->checkLifespan(((float)timer.count() / 1000));
                if ((*i)->PhysicsParticle->bSecond)
                {
                    (*i)->PhysicsParticle->bSecond = false;

                    if ((*i)->PhysicsParticle->lifespan < 1.0f) (*i)->PhysicsParticle->Destroy();
                    timer -= timer;

                }
            }


            /*Draw the results*/
            (*i)->Draw();
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /*Clean the vertex annd buffers*/
    model.cleanUp();

    glfwTerminate();
    return 0;
}