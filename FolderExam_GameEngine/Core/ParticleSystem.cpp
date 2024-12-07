#include "ParticleSystem.h"

#include <glm/gtc/random.hpp>

ParticleSystem::ParticleSystem(int maxParticles)
{
    MaxParticles = maxParticles;
    Possision.reserve(MaxParticles);
    Velocities.reserve(MaxParticles);
    Colours.reserve(MaxParticles);
    Lifespan.reserve(MaxParticles);
}

void ParticleSystem::Update(float deltatime)
{
    for (size_t i = 0; i < Lifespan.size(); i++)
    {
        Velocities[i].y -= gravity * deltatime;
        Possision[i] += Velocities[i] * deltatime;
        Lifespan[i] -= deltatime;

        if (Lifespan[i] <= 0)
        {
            Possision.erase(Possision.begin() + i);
            Velocities.erase(Velocities.begin() + i);
            Colours.erase(Colours.begin() + i);
            Lifespan.erase(Lifespan.begin() + i);
        }
    }
}

void ParticleSystem::render(unsigned int shaderProgram)
{
    glUseProgram(shaderProgram);
    glBindBuffer( GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Possision.size() * sizeof(glm::vec3), Possision.data(), GL_STATIC_DRAW);
    
    glBindVertexArray( VAO);
    glDrawArrays(GL_POINTS, 0, Possision.size());
    glBindVertexArray(0);
    
    
}

void ParticleSystem::emit(glm::vec3 position, glm::vec3 velocity, glm::vec3 colour, float lifespan)
{
 

    if (Possision.size() < MaxParticles)
    {
        glm::vec3 Vel = glm::sphericalRand(8.f);
        Possision.push_back(position);
        Velocities.push_back(Vel);
        Colours.push_back(colour);
        Lifespan.push_back(lifespan);
    }
}

void ParticleSystem::SettupBuffers()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Possision.size() * sizeof(glm::vec3), Possision.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}
