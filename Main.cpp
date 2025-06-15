

#define TINYOBJLOADER_IMPLEMENTATION

/*Shader implementation*/
#include <string>
#include <iostream>

/*P6 and Custom Classes inclusion*/
#include "P6/MyVector.h"
#include "P6/MyParticle.h"
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



bool bPressed = false;
float initialPos = -0.6f;

int totalTrack = (-initialPos + 0.6f) * 0.6;

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
    P6::MyParticle particle = P6::MyParticle("particle");
    RenderParticle rrp = RenderParticle("particle", &particle, &model, P6::MyVector(2.0f, 0.0f, 0.0f));



    //Unique Particle classs and RP identification
   for (int i = 0; i <= 2; i++)
   {
       std::string str = std::to_string(i);
       P6::MyParticle p = particle;
        p.Velocity = P6::MyVector(0, 0, 0);
        p.Position = P6::MyVector((float)(i /10),0.4f,0);
        p.damping = 1;
        p.mass = 1 * i;
  
      /*  P6::ForceGenerator f;
        p.Acceleration = P6::MyVector(1 * i, 0, 0);
        p.addForce(P6::MyVector(1 * i, 0, 0));*/
         //f.updateForce(&particle, 0.1);
       // pWorld.forceRegistry.Add(&p, &f);
        //addd names not the particle mismo
        pWorld.addParticle(&p);
        RenderParticle rp = rrp;
        rp = RenderParticle("particle" + str, &p, &model, P6::MyVector(2.0f, i, 0.0f));
       rParticleList.push_back(&rp);
       

       std::cout << i << std::endl;
   }

   //P6::MyParticle p = P6::MyParticle();
   //RenderParticle p = RenderParticle();

   // /*PARTICLE IMPLEMETATION*/
   //  P6::MyParticle particle = P6::MyParticle("particle" + 1);
   // particle.Velocity = P6::MyVector(0, 0, 0);
   // particle.Position = P6::MyVector(initialPos,0.4f,0);
   // particle.damping = 1;
   // particle.mass = 1;
   //// /*FORCE IMPLEMENTATION*/
   // P6::ForceGenerator force;   
   // particle.Acceleration = P6::MyVector(force.randomForce(30,20), 0, 0);
   // particle.addForce(P6::MyVector(1, 0, 0));
   // force.updateForce(&particle, 0.1);
   // pWorld.forceRegistry.Add(&particle, &force);
   // pWorld.addParticle(&particle);
   // p = RenderParticle("P1", &particle, &model, P6::MyVector(4.0f, 0.0f, 0.0f));
   // rParticleList.push_back(&rp);
   // P6::MyParticle particle2 = P6::MyParticle("particle" + 1);
   // particle2.Velocity = P6::MyVector(0, 0, 0);
   // particle2.Position = P6::MyVector(initialPos, 0.4f, 0);
   // particle2.damping = 1;
   // particle2.mass = 1;
   // // /*FORCE IMPLEMENTATION*/
   // P6::ForceGenerator force2;
   // particle2.Acceleration = P6::MyVector(force2.randomForce(30, 20), 0, 0);
   // particle2.addForce(P6::MyVector(1, 0, 0));
   // force.updateForce(&particle2, 0.1);
   // pWorld.forceRegistry.Add(&particle2, &force2);
   // pWorld.addParticle(&particle);
   //  rp = RenderParticle("P1", &particle2, &model, P6::MyVector(4.0f, 0.0f, 0.0f));
   // rParticleList.push_back(&rp);



    /*SECOND*/
    //P6::MyParticle* p2 = &p;
    //p2->Velocity = P6::MyVector(0, 0, 0);
    //p2->Position = P6::MyVector(initialPos, 0.2f, 0);
    //p2->damping = 1;
    //p2->mass = 1;
    //P6::ForceGenerator f2;
    //p2->Acceleration = P6::MyVector(f2.randomForce(30,20), 0, 0);
    //p2->addForce(P6::MyVector(1, 0, 0));
    //f2.updateForce(p2, 0.1);
    //pWorld.forceRegistry.Add(p2, &f2);
    //pWorld.addParticle(p2);
    //RenderParticle rp2 = RenderParticle("P2", p2, &model, P6::MyVector(0.0f, 0.0f, 4.0f));
    //rParticleList.push_back(&rp2);

    /*THIRD*/
    //P6::MyParticle p3 = P6::MyParticle();
    //p3.Velocity = P6::MyVector(0, 0, 0);
    //p3.Position = P6::MyVector(initialPos,0.0f, 0);
    //p3.damping = 1;
    //p3.mass = 1;
    //P6::ForceGenerator f3;
    //p3.Acceleration = P6::MyVector(f3.randomForce(30,20), 0, 0);
    //p3.addForce(P6::MyVector(1, 0, 0));
    //f3.updateForce(&p3, 0.1);
    //pWorld.forceRegistry.Add(&p3, &f3);
    //pWorld.addParticle(&p3);
    //RenderParticle rp3 = RenderParticle("P3", &p3, &model, P6::MyVector(0.0f, 4.0f, 0.0f));
    //rParticleList.push_back(&rp3);


    /*PLAYER*/
    //P6::MyParticle p = P6::MyParticle();
    //p.Velocity = P6::MyVector(0, 0, 0);
    //p.Position = P6::MyVector(initialPos, -0.4f, 0);
    //p.damping = 1;
    //p.mass = 1;
    //P6::ForceGenerator f;
    //p.Acceleration = P6::MyVector(1, 0, 0);
    //p.addForce(P6::MyVector(1, 0, 0));
    //f.updateForce(&p, 0.1);
    //pWorld.forceRegistry.Add(&p, &f);
    //pWorld.addParticle(&p);
    //RenderParticle rp = RenderParticle("P4 (Player)" ,&p, &model, P6::MyVector(2.0f, 0.0f, 2.0f));
    //rParticleList.push_back(&rp);

    /*TIME IMPLEMENTATION*/
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);
    std::chrono::milliseconds timer(0);
    int ranking = 0;

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

        for (std::list<RenderParticle*>::iterator i = rParticleList.begin();
            i != rParticleList.end(); i++)
        {  
            std::cout << "Position is " << (float)(*i)->PhysicsParticle->mass << std::endl;
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