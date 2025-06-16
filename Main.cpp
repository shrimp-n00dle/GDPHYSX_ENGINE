

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
    

    glm::mat4 projection = glm::ortho(-2.0f, //left most point
        2.0f, //right most point
        -2.0f, //bottom most point
        2.0f, //top most poinr
        -1.f, // Z Near
        1.0f); //Z Far

    /*Shader*/
    Shader modelShader = Shader();

    /*Model*/
   Model model = Model("3D/sphere.obj",modelShader);

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
       
       float color = rand() %10;
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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
       
        /*TIME IMPLEMENTAITION*/
        curr_time = clock::now();

        //Duration checker
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);

        auto timeAdd = std::chrono::duration_cast<std::chrono::milliseconds> (curr_time - prev_time);

        timer += timeAdd;

        prev_time = curr_time;

        //add dur to last iteration to the time since our last frame
        curr_ns += dur;
        if (curr_ns >= timestep)
        {
            //ms converstion
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            //Reset
            curr_ns -= curr_ns;
           pWorld.Update((float)ms.count() / 1000);
        }
       

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        std::cout << (float)timer.count() / 1000 << std::endl;


        for (std::list<RenderParticle*>::iterator i = rParticleList.begin();
            i != rParticleList.end(); i++)
        {  
            /*Check lifespan first*/
            if ((*i)->PhysicsParticle->lifespan > 0.0f)
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