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

// Fountain variables
float particleSpawnRate = 5.0f; // particles per second
float timeSinceLastSpawn = 0.0f;
int maxParticles = 100; // maximum particles alive at once
int particlesPerBurst = 1; // how many particles to spawn at once

// Function to create a single particle
RenderParticle* createParticle(P6::PhysicsWorld* pWorld, Model* model)
{
    /*RANDOM GENERATOR*/
    //color
    float color = rand() % 10;
    float color2 = rand() % 10;
    float color3 = rand() % 10;

    // More fountain-like velocity (upward with some spread)
    float veloX = ((rand() % 100) / 100.0f - 0.5f) * 2.0f; // -1 to 1
    float veloY = (rand() % 50 + 50) / 100.0f * 8.0f + 2.0f; // 2 to 6 (upward)
    float veloZ = ((rand() % 100) / 100.0f - 0.5f) * 2.0f; // -1 to 1

    // Gravity-like acceleration
    float accelX = 0.0f;
    float accelY = -9.81f; // gravity
    float accelZ = 0.0f;

    P6::EngineParticle* p = new P6::EngineParticle();
    p->Velocity = P6::MyVector(veloX, veloY, veloZ);
    p->Position = P6::MyVector(0, -0.7f, 0); // spawn at bottom
    p->Acceleration = P6::MyVector(accelX, accelY, accelZ);

    P6::ForceGenerator* f = new P6::ForceGenerator;
    p->addForce(P6::MyVector(0, 0, 0)); // Let gravity handle the force

    //radius
    p->radius = (float)((float)(rand() % 10 + 2) / (float)(rand() % 10 + 2)) / 10;

    //lifespan (shorter for fountain effect)
    p->lifespan = (rand() % 30 + 10) / 10.0f; // 1.0 to 4.0 seconds

    f->updateForce(p, 0.1f);
    pWorld->forceRegistry.Add(p, f);
    pWorld->addParticle(p);

    RenderParticle* rp = new RenderParticle(p, model, P6::MyVector(color - 3, color2, color3 - 3));
    return rp;
}

// Function to clean up dead particles
void cleanupDeadParticles(std::list<RenderParticle*>& rParticleList)
{
    auto it = rParticleList.begin();
    while (it != rParticleList.end())
    {
        if ((*it)->PhysicsParticle->lifespan <= 0.0f || (*it)->PhysicsParticle->Position.y < -5.0f)
        {
            // Mark particle for destruction (PhysicsWorld will handle removal automatically)
            (*it)->PhysicsParticle->Destroy();

            // Clean up render particle
            delete (*it);

            // Remove from render list
            it = rParticleList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

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

    // Control spawn rate with Q/E keys
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        particleSpawnRate = std::max(1.0f, particleSpawnRate - 5.0f * deltaTime);
        std::cout << "Spawn rate: " << particleSpawnRate << " particles/sec" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        particleSpawnRate = std::min(20.0f, particleSpawnRate + 5.0f * deltaTime);
        std::cout << "Spawn rate: " << particleSpawnRate << " particles/sec" << std::endl;
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
    window = glfwCreateWindow(800, 800, "Group 8 / Fountain Engine", NULL, NULL);
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

    // Get spawn rate from user
    std::cout << "Enter particle spawn rate (particles per second): ";
    std::cin >> particleSpawnRate;

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
            timeSinceLastSpawn += deltaTime;
        }

        prev_time = curr_time;

        // Spawn new particles continuously (fountain effect)
        if (!isPaused && timeSinceLastSpawn >= (1.0f / particleSpawnRate))
        {
            if (rParticleList.size() < maxParticles)
            {
                for (int i = 0; i < particlesPerBurst; i++)
                {
                    RenderParticle* newParticle = createParticle(&pWorld, &model);
                    rParticleList.push_back(newParticle);
                }
            }
            timeSinceLastSpawn = 0.0f;
        }

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

        // Clean up dead particles
        if (!isPaused)
        {
            cleanupDeadParticles(rParticleList);
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

        // Display particle count
        static float displayTimer = 0.0f;
        displayTimer += deltaTime;
        if (displayTimer >= 1.0f) // Update every second
        {
            std::cout << "Active particles: " << rParticleList.size() << " | Spawn rate: " << particleSpawnRate << "/sec" << std::endl;
            displayTimer = 0.0f;
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