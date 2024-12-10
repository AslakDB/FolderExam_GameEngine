#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "glad/glad.h"

class ParticleSystem
{
public:
    ParticleSystem(int maxParticles);
    void Update(float deltatime);
    void render(unsigned int shaderProgram);
    void emit(glm::vec3 position, glm::vec3 velocity, glm::vec3 colour, float lifespan);
    void SettupBuffers();

private:
    std::vector<glm::vec3> Position;
    std::vector<glm::vec3> Velocities;
    std::vector<glm::vec3> Colours;
    std::vector<float> Lifespan;
    int MaxParticles;
    float gravity = 9.81f;
    unsigned int VAO, VBO;
};
