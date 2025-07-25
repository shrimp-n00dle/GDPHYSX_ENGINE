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
constexpr std::chrono::nanoseconds timestep(16ms); // Reduced timestep for smoother simulation

// Camera variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 2.0f;

// Projection variables
bool isPerspective = true;
bool isPaused = false;
bool showDebugInfo = false;
bool wireframeMode = false;

// Newton's cradle parameters - PHYSICS CONSTANTS (independent of visual size)
int numBalls = 5;
float ballRadius = 0.2f;        // Visual and collision radius
float ballMass = 1.0f;          // Mass (independent of radius)
float stringLength = 1.0f;      // String length (constant)
float anchorHeight = 0.8f;      // Anchor height (constant)
float ballSpacing = 0.4f;       // Fixed spacing between ball centers (constant)
float gravityStrength = 9.8f;
float forceX, forceY, forceZ;

// String constraint function
void ApplyStringConstraints(P6::MyParticle* anchor, P6::MyParticle* ball, float stringLength, float deltaTime)
{
    P6::MyVector stringVector = ball->Position - anchor->Position;
    float currentLength = stringVector.Magnitude();

    if (currentLength > 0.001f) // Avoid division by zero
    {
        // Calculate the desired position (maintaining string length)
        P6::MyVector direction = stringVector.scalarMultiplication(1.0f / currentLength);
        P6::MyVector desiredPosition = anchor->Position + direction.scalarMultiplication(stringLength);

        // Calculate position correction
        P6::MyVector positionCorrection = desiredPosition - ball->Position;

        // Apply position constraint with some flexibility for stability
        float constraintStrength = 0.8f; // Allows some flexibility
        ball->Position = ball->Position + positionCorrection.scalarMultiplication(constraintStrength);

        // Apply velocity constraint to maintain string length
        // Project velocity to be perpendicular to string direction
        float velocityAlongString = ball->Velocity.scalarProduct(direction);
        if (velocityAlongString > 0) // Only constrain if moving away from anchor
        {
            P6::MyVector velocityCorrection = direction.scalarMultiplication(-velocityAlongString * 0.9f);
            ball->Velocity = ball->Velocity + velocityCorrection;
        }

        // Add some damping to prevent oscillations
        float dampingFactor = 0.95f;
        ball->Velocity = ball->Velocity.scalarMultiplication(dampingFactor);
    }
}

// Input callback function
void processInput(GLFWwindow* window, float deltaTime)
{
    // WASD Camera movement (orbiting around center point)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos.y += cameraSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos.y -= cameraSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        float angle = cameraSpeed * deltaTime;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 offset = cameraPos - cameraTarget;
        glm::vec4 newOffset = rotation * glm::vec4(offset, 1.0f);
        cameraPos = cameraTarget + glm::vec3(newOffset);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        float angle = -cameraSpeed * deltaTime;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 offset = cameraPos - cameraTarget;
        glm::vec4 newOffset = rotation * glm::vec4(offset, 1.0f);
        cameraPos = cameraTarget + glm::vec3(newOffset);
    }

    // Zoom in/out with Q/E
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(cameraPos - cameraTarget);
        cameraPos += direction * cameraSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        glm::vec3 direction = glm::normalize(cameraPos - cameraTarget);
        cameraPos -= direction * cameraSpeed * deltaTime;
    }

    // Projection switching
    static bool key1Last = false;
    static bool key2Last = false;

    bool key1Now = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS;
    bool key2Now = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS;

    if (key1Now && !key1Last)
    {
        isPerspective = false;
        std::cout << "Switched to ORTHOGRAPHIC projection" << std::endl;
    }
    if (key2Now && !key2Last)
    {
        isPerspective = true;
        std::cout << "Switched to PERSPECTIVE projection" << std::endl;
    }
    key1Last = key1Now;
    key2Last = key2Now;

    // Debug toggle with 'I' key
    static bool keyILast = false;
    bool keyINow = glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS;
    if (keyINow && !keyILast)
    {
        showDebugInfo = !showDebugInfo;
        std::cout << "Debug info " << (showDebugInfo ? "ON" : "OFF") << std::endl;
    }
    keyILast = keyINow;

    // Wireframe toggle with 'F' key
    static bool keyFLast = false;
    bool keyFNow = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
    if (keyFNow && !keyFLast)
    {
        wireframeMode = !wireframeMode;
        if (wireframeMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            std::cout << "Wireframe mode ON" << std::endl;
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            std::cout << "Wireframe mode OFF" << std::endl;
        }
    }
    keyFLast = keyFNow;

    // Spacebar pause toggle
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

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 800, "Group 8 / Newton's Cradle / FOUNTAIN ENGINE", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, 800, 800);
    glEnable(GL_DEPTH_TEST);

    /*Shader and Model setup*/
    Shader modelShader = Shader();
    Model model = Model("3D/sphere.obj", modelShader);
    model.bind();

    /*PHYSICS WORLD IMPLEMENTATION*/
    P6::PhysicsWorld pWorld = P6::PhysicsWorld();
    std::list<RenderParticle*> rParticleList;

    //Ask for input first
    std::cout << "Cable Length: "; std::cin >> stringLength;
    std::cout << "Particle Gap: "; std::cin >> ballSpacing;
    std::cout << "Particle Radius: "; std::cin >> ballRadius;
    std::cout << "Gravity Strength: "; std::cin >> gravityStrength;
    std::cout << "Apply Force" << std::endl << "X :";  std::cin >> forceX;
    std::cout << "Y: "; std::cin >> forceY;
    std::cout << "Z: "; std::cin >> forceZ;


    // ===== NEWTON'S CRADLE SETUP =====
    const int NUM_BALLS = numBalls;
    const float BALL_RADIUS = ballRadius/100.f;          // Visual and collision radius
    const float BALL_MASS = ballMass/100.f;              // Mass (independent of radius)
    const float STRING_LENGTH = stringLength/100.0f;      // Fixed string length
    const float ANCHOR_HEIGHT = anchorHeight;      // Fixed anchor height
    const float BALL_SPACING = ballSpacing/100.0f; 

    // Ball spacing is now constant, independent of ball radius

    std::cout << "=== NEWTON'S CRADLE SETUP ===" << std::endl;
    std::cout << "Number of balls: " << NUM_BALLS << std::endl;
    std::cout << "Ball radius: " << BALL_RADIUS << std::endl;
    std::cout << "Ball spacing: " << BALL_SPACING << std::endl;
    std::cout << "String length: " << STRING_LENGTH << std::endl;
    std::cout << "Anchor height: " << ANCHOR_HEIGHT << std::endl;
    std::cout << "=============================" << std::endl;

    // Arrays to hold our particles
    P6::MyParticle* anchors[5];
    P6::MyParticle* balls[5];
    RenderParticle* renderAnchors[5];
    RenderParticle* renderBalls[5];

    // Create the Newton's cradle
    for (int i = 0; i < NUM_BALLS; i++)
    {
        // Use fixed spacing instead of radius-dependent spacing
        float xPos = (i - 2) * BALL_SPACING; // Center the cradle

        // Create anchor points (fixed points that don't move)
        anchors[i] = new P6::MyParticle();
        anchors[i]->Position = P6::MyVector(xPos, ANCHOR_HEIGHT, 0);
        anchors[i]->mass = 0.0f; // Zero mass = immovable (infinite mass)
        anchors[i]->radius = 0.05f; // Small visual radius for anchors
        anchors[i]->restitution = 0.1f;
        anchors[i]->Velocity = P6::MyVector(0, 0, 0);
        anchors[i]->setInverseMass(0.0f); // This makes them immovable

        // Don't add anchors to physics world since they shouldn't be affected by forces
        renderAnchors[i] = new RenderParticle("Anchor" + std::to_string(i), anchors[i], &model, P6::MyVector(0.5f, 0.5f, 0.5f));
        renderAnchors[i]->model->scaleModel(P6::MyVector(anchors[i]->radius, anchors[i]->radius, anchors[i]->radius));
        rParticleList.push_back(renderAnchors[i]);

        // Create the hanging balls at rest position
        balls[i] = new P6::MyParticle();
        balls[i]->Position = P6::MyVector(xPos, ANCHOR_HEIGHT - STRING_LENGTH, 0);
        balls[i]->mass = BALL_MASS;                    // Fixed mass
        balls[i]->radius = BALL_RADIUS;                // Set particle radius to match visual radius
        balls[i]->restitution = 2.0f;                 // High restitution for good energy transfer
       
        if (i == 0) balls[i]->Velocity = P6::MyVector(forceX, forceY, forceZ);
        else balls[i]->Velocity = P6::MyVector(0, 0, 0);

        pWorld.addParticle(balls[i]);

        // Different colors for each ball
        P6::MyVector ballColor;
        switch (i) {
        case 0: ballColor = P6::MyVector(1.0f, 0.2f, 0.2f); break; // Red
        case 1: ballColor = P6::MyVector(0.2f, 1.0f, 0.2f); break; // Green
        case 2: ballColor = P6::MyVector(0.2f, 0.2f, 1.0f); break; // Blue
        case 3: ballColor = P6::MyVector(1.0f, 1.0f, 0.2f); break; // Yellow
        case 4: ballColor = P6::MyVector(1.0f, 0.2f, 1.0f); break; // Magenta
        default: ballColor = P6::MyVector(1.0f, 1.0f, 1.0f); break;
        }

        renderBalls[i] = new RenderParticle("Ball" + std::to_string(i), balls[i], &model, ballColor);
        renderBalls[i]->model->scaleModel(P6::MyVector(balls[i]->radius, balls[i]->radius, balls[i]->radius));
        rParticleList.push_back(renderBalls[i]);
    }

    // Add very light damping for realism
    P6::DragForceGenerator drag = P6::DragForceGenerator(0.005f, 0.005f);
    for (int i = 0; i < NUM_BALLS; i++)
    {
        pWorld.forceRegistry.Add(balls[i], &drag);
    }

    /*TIME IMPLEMENTATION*/
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    auto lastFrameTime = clock::now();



    // Print controls
    std::cout << "=== NEWTON'S CRADLE CONTROLS ===" << std::endl;
    std::cout << "WASD: Camera movement" << std::endl;
    std::cout << "Q/E: Zoom out/in" << std::endl;
    std::cout << "1: Orthographic projection" << std::endl;
    std::cout << "2: Perspective projection" << std::endl;
    std::cout << "I: Toggle debug information" << std::endl;
    std::cout << "F: Toggle wireframe mode" << std::endl;
    std::cout << "SPACE: Pause/Resume simulation" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    std::cout << "===============================" << std::endl;

    int debugFrameCounter = 0;

    /* Main loop */
    while (!glfwWindowShouldClose(window))
    {
        curr_time = clock::now();

        // Calculate delta time for camera movement
        auto deltaTimeDuration = curr_time - lastFrameTime;
        float deltaTime = std::chrono::duration<float>(deltaTimeDuration).count();
        lastFrameTime = curr_time;

        processInput(window, deltaTime);

        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
        prev_time = curr_time;

        if (!isPaused)
        {
            curr_ns += dur;
            if (curr_ns >= timestep)
            {
                auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
                curr_ns -= curr_ns;

                float physicsTimeStep = (float)ms.count() / 1000.0f;

                // Update physics world first
                pWorld.UpdateWithoutCollisions(physicsTimeStep);

                // Apply string constraints AFTER physics update
                for (int i = 0; i < NUM_BALLS; i++)
                {
                    ApplyStringConstraints(anchors[i], balls[i], STRING_LENGTH, physicsTimeStep);
                }

                // FIXED COLLISION HANDLING between balls
                const int collisionIterations = 2;
                for (int iter = 0; iter < collisionIterations; ++iter)
                {
                    bool hadCollision = false;

                    for (int i = 0; i < NUM_BALLS - 1; ++i)
                    {
                        P6::MyVector delta = balls[i + 1]->Position - balls[i]->Position;
                        float dist = delta.Magnitude();

                        // FIXED: Use exact sum of VISUAL radii for collision detection
                        // Use the actual ball radius we set, not the internal particle radius
                        float minDist = BALL_RADIUS + BALL_RADIUS;

                        if (dist < minDist && dist > 0.001f)
                        {
                            hadCollision = true;

                            P6::MyVector collisionNormal = delta.scalarMultiplication(1.0f / dist);

                            // Get relative velocity
                            P6::MyVector relativeVelocity = balls[i + 1]->Velocity - balls[i]->Velocity;
                            float relativeVelNormal = relativeVelocity.scalarProduct(collisionNormal);

                            // Only resolve if balls are approaching
                            if (relativeVelNormal < 0)
                            {
                                float restitution = 0.95f; // High restitution for Newton's cradle
                                float impulse = -(1.0f + restitution) * relativeVelNormal;
                                impulse /= (balls[i]->getInverseMass() + balls[i + 1]->getInverseMass());

                                P6::MyVector impulseVector = collisionNormal.scalarMultiplication(impulse);

                                balls[i]->Velocity = balls[i]->Velocity - impulseVector.scalarMultiplication(balls[i]->getInverseMass());
                                balls[i + 1]->Velocity = balls[i + 1]->Velocity + impulseVector.scalarMultiplication(balls[i + 1]->getInverseMass());

                                if (showDebugInfo && debugFrameCounter % 30 == 0)
                                {
                                    std::cout << "Collision between ball " << i << " and " << (i + 1)
                                        << " at distance: " << dist << " (minDist: " << minDist << ")"
                                        << " Ball radius used: " << BALL_RADIUS
                                        << " Particle radius: " << balls[i]->radius << std::endl;
                                }
                            }

                            // Position correction to prevent sinking
                            float overlap = minDist - dist;
                            if (overlap > 0)
                            {
                                float correction = overlap * 0.5f;
                                P6::MyVector correctionVector = collisionNormal.scalarMultiplication(correction);
                                balls[i]->Position = balls[i]->Position - correctionVector;
                                balls[i + 1]->Position = balls[i + 1]->Position + correctionVector;
                            }
                        }
                    }

                    if (!hadCollision) break;
                }

                // Debug information
                if (showDebugInfo && debugFrameCounter % 90 == 0)
                {
                    std::cout << "\n=== DEBUG INFO ===" << std::endl;
                    for (int i = 0; i < NUM_BALLS; i++)
                    {
                        P6::MyVector pos = balls[i]->Position;
                        P6::MyVector vel = balls[i]->Velocity;
                        P6::MyVector anchorDist = balls[i]->Position - anchors[i]->Position;
                        float stringLen = anchorDist.Magnitude();

                        std::cout << "Ball " << i << " - Pos: (" << pos.x << ", " << pos.y << ", " << pos.z
                            << ") Vel: (" << vel.x << ", " << vel.y << ", " << vel.z
                            << ") String Length: " << stringLen
                            << " Radius: " << balls[i]->radius << std::endl;
                    }
                    std::cout << "===================" << std::endl;
                }
                debugFrameCounter++;
            }
        }

        /* Render */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(modelShader.shaderProg);

        // Create projection matrix
        glm::mat4 projection;
        if (isPerspective)
        {
            projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
        }
        else
        {
            projection = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, -10.0f, 10.0f);
        }

        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

        unsigned int projLoc = glGetUniformLocation(modelShader.shaderProg, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        unsigned int viewLoc = glGetUniformLocation(modelShader.shaderProg, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Draw all particles
        for (std::list<RenderParticle*>::iterator i = rParticleList.begin();
            i != rParticleList.end(); i++)
        {
            (*i)->Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    model.cleanUp();

    for (int i = 0; i < NUM_BALLS; i++)
    {
        delete anchors[i];
        delete balls[i];
        delete renderAnchors[i];
        delete renderBalls[i];
    }

    glfwTerminate();
    return 0;
}