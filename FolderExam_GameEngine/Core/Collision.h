#pragma once
#include "Model.h"
#include "Components.h"
#include "Systems.h"

class Collision
{
public:
    
    /// collisjon mellom alle spheres i prosjektet
    /// @param sphere_models
    bool SphereSphereCollision(glm::vec3 playerPos,glm::vec3 otherPos);

    
    /// collision sjekk mellom Sphere og AABB
    /// @param sphere_models 
    /// @param other_models 
    void SphereBoxCollision(const std::vector<model*>& sphere_models, const std::vector<model*>& other_models);


    /// En gammel collsison check som er basert AABB collision (ikke i bruk)
    /// @param AllModels 
    void CollisionCheck(std::vector<model*> AllModels);

  
};

struct collision_component : public Components
{
    glm::vec3 distanceVector = glm::vec3(0.f);//playerPos - otherPos;
    float distanceSquared = 0;//glm::dot(distanceVector, distanceVector);
    float radiusSum = 0;// 2.0f * radius;
    float radiusSumSquared =0;// radiusSum * radiusSum;
    glm::vec3 collisionNormal;
        
};

struct collision_system : public Systems
{
        void SphereSphereCollision(component_manager& componentManager)
        {
            auto* collision = componentManager.get_component_handler<collision_component>();
            auto* transform = componentManager.get_component_handler<transform_component>();
            auto* movement = componentManager.get_component_handler<movement_component>();

            if (!collision || !transform || !movement) {
                return;
            }

            auto& coll = collision->Components;
            auto& transforms = transform->Components;
            auto& movements = movement->Components;
            
            for (size_t i = 0; i < coll.size(); ++i) {
                for (size_t j = i + 1; j < coll.size(); ++j) {
                    coll[i].distanceVector = transforms[i].PlayerPos - transforms[j].PlayerPos;
                    coll[i].distanceSquared = glm::dot(coll[i].distanceVector, coll[i].distanceVector);
                    coll[i].radiusSum = 2.0f * 0.5f;
                    coll[i].radiusSumSquared = coll[i].radiusSum * coll[i].radiusSum;

                    if (coll[i].distanceSquared < coll[i].radiusSumSquared) {
                        std::cout << "Collision" << '\n';
                        coll[i].collisionNormal = glm::normalize(coll[i].distanceVector);
                        glm::vec3 relativeVelocity = movements[i].Velocity - movements[j].Velocity;
                        float velocityAlongNormal = glm::dot(relativeVelocity, coll[i].collisionNormal);
                        if (velocityAlongNormal < 0) {
                            glm::vec3 impulse = (-(1.0f + 1.0f) * velocityAlongNormal * coll[i].collisionNormal);
                            movements[i].Velocity += impulse * 0.8f;
                            movements[j].Velocity -= impulse * 0.8f;
                        }
                    }
                }
            }
        }
        
        void Update(unsigned int ShaderProgram, component_manager& componentManager, float deltatime) override
        {
            SphereSphereCollision(componentManager);
        }
     
};