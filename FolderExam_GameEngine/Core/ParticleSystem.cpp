#include "ParticleSystem.h"

#include <glm/gtc/random.hpp>

#include "Vertex.h"

ParticleSystem::ParticleSystem(int maxParticles)
{
    MaxParticles = maxParticles;
    Position.reserve(MaxParticles);
    Velocities.reserve(MaxParticles);
    Colours.reserve(MaxParticles);
    Lifespan.reserve(MaxParticles);
}

void ParticleSystem::Update(float deltatime)
{
    for (size_t i = 0; i < Lifespan.size(); i++)
    {
        Velocities[i].y -= gravity * deltatime;
        Position[i] += Velocities[i] * deltatime;
        Lifespan[i] -= deltatime;

        if (Lifespan[i] <= 0)
        {
            Position.erase(Position.begin() + i);
            Velocities.erase(Velocities.begin() + i);
            Colours.erase(Colours.begin() + i);
            Lifespan.erase(Lifespan.begin() + i);
        }
    }
}

void ParticleSystem::render(unsigned int shaderProgram)
{

    std::vector<Vertex> vertices;
    for (int i = 0; i < Position.size(); ++i)
    {
        vertices.emplace_back(Position[i], glm::vec3(1.f), Colours[i]);
    }
    
    glUseProgram(shaderProgram);
    glBindBuffer( GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
    
    glBindVertexArray( VAO);
    glDrawArrays(GL_POINTS, 0, vertices.size());
    glBindVertexArray(0);
    
    
}

void ParticleSystem::emit(glm::vec3 position, glm::vec3 velocity, glm::vec3 colour, float lifespan)
{
    
    if (Position.size() < MaxParticles)
    {
        Position.push_back(position);
        Velocities.push_back(velocity);
        Colours.push_back(colour);
        Lifespan.push_back(lifespan);
    }
}

void ParticleSystem::SettupBuffers()
{
    std::vector<Vertex> vertices;
    for (int i = 0; i < Position.size(); ++i)
    {
        vertices.emplace_back(Position[i], glm::vec3(1.f), Colours[i]);
    }
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 3, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 3, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    

    glBindVertexArray(0);
}
