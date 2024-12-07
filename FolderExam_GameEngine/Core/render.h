#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/random.hpp>
#include "Camera.h"
#include "Model.h"
#include "plane.h"
#include "Sphere.h"
#include "Collision.h"
#include "Entity.h"
#include "Systems.h"
#include "Components.h"
#include "ParticleSystem.h"


Sphere sphere;
Plane Plane;
Collision coll;
ParticleSystem particleSystem(5000);
Camera camera;

entity_manager entityManager;
component_manager componentManager;
SystemManager systemManager;
render_system renderSystem;
collision_system collisionSystem;


bool firstMouse = true;

float lastX = 960, lastY = 540;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void ProsessInput(GLFWwindow *window, float deltaTime, movement_component& MVM, Entity& Player);

void CreateEnemy(int numOfEnemies);

struct Render {
bool inside;
    bool isMovingForward;
    
    Render() = default;
    void render(GLFWwindow* window, unsigned int shaderProgram, float deltaTime, float lastFrame) {
        
        renderSystem.CreateMeshes();

        std::vector<glm::vec3> EnemyPos;
        std::vector<Entity> Enemies;


        particleSystem.SettupBuffers();
     
        
        /*Entity Player = entityManager.create_entity();
        if (Player.ID == -1){std::cerr << "Failed to create entity." << std::endl;}
        */

        
        /*
        Entity Enemy0 = entityManager.create_entity();
        if (Enemy0.ID == -1){std::cerr << "Failed to create enemy entity." << std::endl;}
        Entity Enemy1 = entityManager.create_entity();
       if (Enemy1.ID == -1){std::cerr << "Failed to create enemy entity." << std::endl;}
       */
        Entity Player(0);
        Entity Plane0(1);
        Entity Enemy0(2);
        Entity Enemy1(3);
        
        /*Entity Plane0 = entityManager.create_entity();
        if (Plane0.ID == -1){std::cerr << "Failed to create plane entity." << std::endl;}*/

        

       
        systemManager.AddSystem<matrix_system>();
        //systemManager.AddSystem<collision_system>();
        //systemManager.AddSystem<plane_system>(Plane0.ID);
        systemManager.AddSystem<render_system>();
        systemManager.AddSystem<movementSystem>();
        systemManager.AddSystem<HealthSystem>();

       // systemManager.AddSystem<test_system>(TestEntity.ID);

        
        // Add components
        componentManager.add_component<model_component>(Player.ID);
        componentManager.add_component<health_component>(Player.ID);
        componentManager.add_component<transform_component>(Player.ID);
        componentManager.add_component<collision_component>(Player.ID);
        
        componentManager.add_component<movement_component>(Player.ID);
        componentManager.add_component<mesh_component>(Player.ID);
        componentManager.add_component<sphere_component>(Player.ID);
        componentManager.add_component<matrix_component>(Player.ID);
        componentManager.getComponent<model_component>(Player.ID).MeshName = "Sphere";
        componentManager.getComponent<model_component>(Player.ID).colour = glm::vec3(1,0,0);
        componentManager.getComponent<health_component>(Player.ID).health = 3;
       // componentManager.getComponent<movement_component>(Sphere0.ID).Velocity = glm::vec3(2, 0, 0) ;

        
        componentManager.add_component<model_component>(Plane0.ID);
        componentManager.add_component<plane_component>(Plane0.ID);
       // componentManager.add_component<mesh_component>(Plane0.ID);
        componentManager.add_component<transform_component>(Plane0.ID);
       // componentManager.add_component<movement_component>(Plane0.ID);
        componentManager.add_component<matrix_component>(Plane0.ID);

        
        componentManager.getComponent<model_component>(Plane0.ID).MeshName = "Plane";
        componentManager.getComponent<transform_component>(Plane0.ID).Scale = glm::vec3(2, 1, 2);
        componentManager.getComponent<transform_component>(Plane0.ID).PlayerPos = glm::vec3(0, 0, 0);
        
        
        
        

        componentManager.getComponent<transform_component>(Player.ID).PlayerPos = glm::vec3(0,0 , 2);
        componentManager.getComponent<transform_component>(Player.ID).Scale =  glm::vec3(0.5, 0.5,0.5);

        
        componentManager.add_component<model_component>(Enemy0.ID);
        componentManager.add_component<transform_component>(Enemy0.ID);
        componentManager.add_component<movement_component>(Enemy0.ID);
        componentManager.add_component<collision_component>(Enemy0.ID);
        componentManager.add_component<mesh_component>(Enemy0.ID);
        componentManager.add_component<sphere_component>(Enemy0.ID);
        componentManager.add_component<matrix_component>(Enemy0.ID);
        componentManager.add_component<health_component>(Enemy0.ID);
        
        componentManager.getComponent<health_component>(Enemy0.ID).health = 2;
        componentManager.getComponent<model_component>(Enemy0.ID).MeshName = "Sphere";
        componentManager.getComponent<transform_component>(Enemy0.ID).Scale =  glm::vec3(0.5, 0.5,0.5);
        componentManager.getComponent<transform_component>(Enemy0.ID).PlayerPos = glm::vec3(-3,0 , -2);


        componentManager.add_component<model_component>(Enemy1.ID);
        componentManager.add_component<transform_component>(Enemy1.ID);
        componentManager.add_component<movement_component>(Enemy1.ID);
        componentManager.add_component<collision_component>(Enemy1.ID);
        componentManager.add_component<mesh_component>(Enemy1.ID);
        componentManager.add_component<sphere_component>(Enemy1.ID);
        componentManager.add_component<matrix_component>(Enemy1.ID);
        componentManager.add_component<health_component>(Enemy1.ID);
        
        componentManager.getComponent<health_component>(Enemy1.ID).health = 2;
        componentManager.getComponent<model_component>(Enemy1.ID).MeshName = "Sphere";
        componentManager.getComponent<transform_component>(Enemy1.ID).Scale =  glm::vec3(0.5, 0.5,0.5);
        componentManager.getComponent<transform_component>(Enemy1.ID).PlayerPos = glm::vec3(3,0 , -2);
        
        


        // Setup sphere model radius
        auto* sphereHandler = static_cast<component_handler<sphere_component>*>(componentManager.component_map[typeid(sphere_component).name()]);
        sphereHandler->Components[sphereHandler->index[Player.ID]].radius = 0.5f;
      
        
     Enemies.emplace_back(Enemy0);
        Enemies.emplace_back(Enemy1);

        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 projection;

        EnemyPos.emplace_back(componentManager.getComponent<transform_component>(Enemy0.ID).PlayerPos);
                   EnemyPos.emplace_back(componentManager.getComponent<transform_component>(Enemy1.ID).PlayerPos);

       
        while (!glfwWindowShouldClose(window))
            {
            particleSystem.emit( glm::vec3(0, 70, 0), glm::sphericalRand(8), glm::vec3(1, 0, 0), 20);
            particleSystem.Update(deltaTime);
            
            
         
            /*glm::vec3 Enemy0direction = glm::normalize(componentManager.getComponent<transform_component>(Player.ID).PlayerPos - componentManager.getComponent<transform_component>(Enemy0.ID).PlayerPos);
            componentManager.getComponent<movement_component>(Enemy0.ID).Velocity = Enemy0direction * 0.5f;
            */

            glm::vec3 Enemy1direction = glm::normalize(componentManager.getComponent<transform_component>(Player.ID).PlayerPos - componentManager.getComponent<transform_component>(Enemy1.ID).PlayerPos);
            componentManager.getComponent<movement_component>(Enemy1.ID).Velocity = Enemy1direction * 0.5f;
            

            if (componentManager.getComponent<health_component>(Player.ID).health <= 0)
            {
                std::cout << "Player is dead" << std::endl;
                //break;
            }

            

            EnemyPos[0] = componentManager.getComponent<transform_component>(Enemy0.ID).PlayerPos;
            EnemyPos[1] = componentManager.getComponent<transform_component>(Enemy1.ID).PlayerPos;
            

            
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            ProsessInput(window, deltaTime, componentManager.getComponent<movement_component>(Player.ID) , Player);
            
            
            //systemManager.UpdateSystems<model_system>(shaderProgram,componentManager);
            
            projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

            camera.tick(shaderProgram);

       
            glClearColor(0.5f, 0.99f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            int viewLoc = glGetUniformLocation(shaderProgram, "viewPos");
            glUniform3fv(viewLoc, 1, glm::value_ptr(camera.cameraPos));

            int LightLoc = glGetUniformLocation(shaderProgram, "lightPos");
            glUniform3fv(LightLoc, 1, glm::value_ptr(glm::vec3(0,10,0)));

            glPointSize(5);
            glLineWidth(3);
            

            particleSystem.render(shaderProgram);
            //systemManager.UpdateSystems<render_system>(shaderProgram, componentManager);
            renderSystem.Update(shaderProgram, componentManager, deltaTime);
            systemManager.UpdateSystems<matrix_system>(shaderProgram, componentManager, deltaTime);
            
            systemManager.UpdateSystems<movementSystem>(shaderProgram,componentManager, deltaTime);
           // collisionSystem.Update(shaderProgram, componentManager, deltaTime);
            
            glfwSwapBuffers(window);
            glfwPollEvents();
            }
    }
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.yaw   += xoffset;
    camera.pitch += yoffset;

    if(camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.cameraFront = glm::normalize(direction);
}


void ProsessInput(GLFWwindow *window, float deltaTime, movement_component& MVM, Entity& Player)
{
    glm::vec3 cameraFrontXZ = glm::normalize(glm::vec3(camera.cameraFront.x, 0.0f, camera.cameraFront.z));

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime ;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.cameraPos += cameraSpeed * cameraFrontXZ;
    }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        { camera.cameraPos -= cameraSpeed * cameraFrontXZ;
            /*Player.PlayerPos -=  cameraSpeed * cameraFrontXZ;*/}
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
            /*Player.PlayerPos -=  glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;*/}
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
            /*Player.PlayerPos +=  glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;*/}
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.cameraPos += cameraSpeed * camera.cameraUp; // Move camera up
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.cameraPos -= cameraSpeed * camera.cameraUp;

        if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        {
            std::cout<<"Healing"<<std::endl;
            componentManager.getComponent<health_component>(Player.ID).health = 3;
            std::cout << "Player health: " << componentManager.getComponent<health_component>(Player.ID).health << std::endl;
        }

    
    
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        deltaTime = 0;
    }
    
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        MVM.Velocity -= glm::vec3(0, 0, 2) * deltaTime;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        MVM.Velocity += glm::vec3(0, 0, 2) * deltaTime;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        MVM.Velocity -= glm::vec3(2, 0, 0) * deltaTime;
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        MVM.Velocity += glm::vec3(2, 0,0 ) * deltaTime;
    } else {
        MVM.Velocity = glm::vec3(0, 0, 0);
    }
}


