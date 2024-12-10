#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>
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

extern "C"
{
#include "../lua54/include/lua.h"
#include "../lua54/include/lauxlib.h"
#include "../lua54/include/lualib.h"
}

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
gravity_system gravitySystem;
DOD_HealthSystem DOD_healthSystem;


bool firstMouse = true;

float lastX = 960, lastY = 540;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void ProsessInput(GLFWwindow *window, float deltaTime, movement_component& MVM, Entity& Player, lua_State* L);

void CreateBall(int ID, float size);

void LoadAndRunSpawnBallsScript(lua_State* L) {
    // Load and run the Lua script
    if (luaL_dofile(L, "../spawnBalls.lua") != LUA_OK) {
        // Print error if script fails to load or run
        std::cerr << "Error running spawnBalls.lua: " 
                  << lua_tostring(L, -1) << std::endl;
    }
}
int LuaCreateBall(lua_State* L) {
    // Check that we received an integer argument
    int enemyID = luaL_checkinteger(L, 1);
    float ballSize = luaL_checknumber(L, 2);
    // Call the CreateEnemy function
    CreateBall(enemyID, ballSize);
    
    // No return values
    return 0;
}

void SetupLuaBindings(lua_State* L) {
    // Register the function so it can be called from Lua
    lua_register(L, "CreateEnemy", LuaCreateBall);
}
lua_State* SetupLuaState() {
    // Create a new Lua state
    lua_State* L = luaL_newstate();
    
    // Load standard Lua libraries
    luaL_openlibs(L);
    
    // Register the CreateEnemy function so it's callable from Lua
    lua_register(L, "CreateEnemy", LuaCreateBall);
    
    return L;
}

struct Render {
bool inside;
    bool isMovingForward;
    
    Render() = default;
    void render(GLFWwindow* window, unsigned int shaderProgram, float deltaTime, float lastFrame) {

        static lua_State* L = SetupLuaState();
    
        // Register bindings only once
        static bool bindingsSetup = false;
        if (!bindingsSetup) {
            SetupLuaBindings(L);
            bindingsSetup = true;
        }

        // Load and run the Lua script
        
        
        renderSystem.CreateMeshes();

        std::vector<glm::vec3> EnemyPos;
        


        particleSystem.SettupBuffers();
        
        
        
        
        LoadAndRunSpawnBallsScript(L);  
        
       
        
        Entity Plane0(0);
        CreateBall(2, 0.5f);
        CreateBall(3, 0.5f);
       
        systemManager.AddSystem<matrix_system>();
        //systemManager.AddSystem<collision_system>();
        //systemManager.AddSystem<plane_system>(Plane0.ID);
        systemManager.AddSystem<render_system>();
        systemManager.AddSystem<movementSystem>();
        systemManager.AddSystem<HealthSystem>();
        systemManager.AddSystem<gravity_system>();

       // systemManager.AddSystem<test_system>(TestEntity.ID);

        Entity Player(1);
        // Add components
        componentManager.add_component<model_component>(Player.ID);
        //componentManager.add_component<health_component>(Player.ID);
        
        componentManager.add_component<DOD_health_component>(Player.ID);
        DOD_health_component& healthComp = componentManager.getComponent<DOD_health_component>(Player.ID);
        healthComp.health.push_back(100); // Add a health value
         // Modify the health value;
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

        
        
        componentManager.getComponent<transform_component>(Player.ID).PlayerPos = glm::vec3(0,0 , 2);
        componentManager.getComponent<transform_component>(Player.ID).Scale =  glm::vec3(0.5, 0.5,0.5);

        
        
         
        
       
        componentManager.getComponent<transform_component>(2).PlayerPos.x = 5.f;
        
        
        
        componentManager.add_component<model_component>(Plane0.ID);
        componentManager.add_component<plane_component>(Plane0.ID);
        // componentManager.add_component<mesh_component>(Plane0.ID);
        componentManager.add_component<transform_component>(Plane0.ID);
       // componentManager.add_component<movement_component>(Plane0.ID);
        componentManager.add_component<matrix_component>(Plane0.ID);

        
        componentManager.getComponent<model_component>(Plane0.ID).MeshName = "Plane";
        componentManager.getComponent<model_component>(Plane0.ID).colour = glm::vec3(1.f);
        componentManager.getComponent<transform_component>(Plane0.ID).Scale = glm::vec3(2, 1, 2);
        componentManager.getComponent<transform_component>(Plane0.ID).PlayerPos = glm::vec3(0, 0, 0);


        // Setup sphere model radius
        auto* sphereHandler = static_cast<component_handler<sphere_component>*>(componentManager.component_map[typeid(sphere_component).name()]);
        sphereHandler->Components[sphereHandler->index[Player.ID]].radius = 0.5f;
      
        
     

        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 projection;

        EnemyPos.emplace_back(componentManager.getComponent<transform_component>(2).PlayerPos);
                   EnemyPos.emplace_back(componentManager.getComponent<transform_component>(3).PlayerPos);

       
        while (!glfwWindowShouldClose(window))
            {
            
            auto& planeTransform = componentManager.getComponent<transform_component>(Plane0.ID);
            planeTransform.PlayerPos.y = std::max<float>(planeTransform.PlayerPos.y, 0);
            planeTransform.PlayerPos.x = std::max<float>(planeTransform.PlayerPos.x, 0);
            planeTransform.PlayerPos.z = std::max<float>(planeTransform.PlayerPos.z, 0);
            
                       
            if (componentManager.getComponent<health_component>(Player.ID).health <= 0)
            {
                std::cout << "Player is dead" << std::endl;
                //break;
            }
            
            
            
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            ProsessInput(window, deltaTime, componentManager.getComponent<movement_component>(Player.ID) , Player, L);
            
            
            //systemManager.UpdateSystems<model_system>(shaderProgram,componentManager);
            
            projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

            camera.tick(shaderProgram);

       
            glClearColor(0.5f, 0.99f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            int viewLoc = glGetUniformLocation(shaderProgram, "viewPos");
            glUniform3fv(viewLoc, 1, glm::value_ptr(camera.cameraPos));

            int LightLoc = glGetUniformLocation(shaderProgram, "lightPos");
            glUniform3fv(LightLoc, 1, glm::value_ptr(glm::vec3(0,50,0)));

            glPointSize(5);
            glLineWidth(3);
            
            particleSystem.emit( glm::vec3(0, 45, 0), glm::sphericalRand(8.f), glm::vec3(1.f,1.f,1.f), 10);
            particleSystem.Update(deltaTime);
            particleSystem.render(shaderProgram);
            //systemManager.UpdateSystems<render_system>(shaderProgram, componentManager);
            renderSystem.Update(shaderProgram, componentManager, deltaTime);
            DOD_healthSystem.Update(shaderProgram, componentManager, deltaTime);
            systemManager.UpdateSystems<matrix_system>(shaderProgram, componentManager, deltaTime);
            gravitySystem.Update(shaderProgram, componentManager, deltaTime);
            systemManager.UpdateSystems<movementSystem>(shaderProgram,componentManager, deltaTime);
            collisionSystem.Update(shaderProgram, componentManager, deltaTime);
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


void ProsessInput(GLFWwindow *window, float deltaTime, movement_component& MVM, Entity& Player, lua_State* L)
{
    glm::vec3 cameraFrontXZ = glm::normalize(glm::vec3(camera.cameraFront.x, 0.0f, camera.cameraFront.z));

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime ;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        cameraSpeed = 7.5f * deltaTime ;
    }
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
        LoadAndRunSpawnBallsScript(L);
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

inline void CreateBall(int ID, float size)
{
    Entity Ball(ID);
    componentManager.add_component<model_component>(Ball.ID);
    componentManager.add_component<transform_component>(Ball.ID);
    componentManager.add_component<movement_component>(Ball.ID);
    componentManager.add_component<collision_component>(Ball.ID);
    componentManager.add_component<mesh_component>(Ball.ID);
    componentManager.add_component<sphere_component>(Ball.ID);
    componentManager.add_component<matrix_component>(Ball.ID);
    componentManager.add_component<health_component>(Ball.ID);
    componentManager.getComponent<model_component>(Ball.ID).MeshName = "Sphere";
    componentManager.getComponent<health_component>(Ball.ID).health = 2;
    componentManager.getComponent<transform_component>(Ball.ID).Scale =  glm::vec3(size);
    componentManager.getComponent<transform_component>(Ball.ID).PlayerPos = glm::vec3(ID,10.f ,0.f);
    
}

