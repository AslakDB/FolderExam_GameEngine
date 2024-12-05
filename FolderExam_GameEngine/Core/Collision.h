#pragma once
#include "Model.h"
#include "Components.h"
#include "render.h"
#include "Systems.h"

class Collision
{
public:
    /// collisjon mellom alle spheres i prosjektet
    /// @param sphere_models
    bool SphereSphereCollision(glm::vec3 playerPos,glm::vec3 otherPos);

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
        void SphereSphereCollision(component_manager& componentManager,std::vector<glm::vec3> entities)
        {
            component_handler<collision_component> *collision = componentManager.get_component_handler<collision_component>();
            component_handler<transform_component> *transform = componentManager.get_component_handler<transform_component>();
            auto& coll = collision->Components;
            auto& transforms = transform->Components;

            for (int i = 0; i < entities.size() -1; ++i)
            {
                for (int j = i +1; j < entities.size(); ++j)
                {
                    coll[i].distanceVector = transforms[i].PlayerPos - transforms[j].PlayerPos;
                    coll[i].distanceSquared = glm::dot(coll[i].distanceVector, coll[i].distanceVector);
                    coll[i].radiusSum = 2.0f * 0.5f;
                    coll[i].radiusSumSquared = coll[i].radiusSum * coll[i].radiusSum;

                    if (coll[i].distanceSquared < coll[i].radiusSumSquared)
                    {
                        glm::vec3 collisionNormal = glm::normalize(coll[i].distanceVector);
                        
                    }
                }
            }
        }
        void Update(unsigned int ShaderProgram, component_manager& componentManager, float deltatime) override;
    };
    /// collision sjekk mellom Sphere og AABB
    /// @param sphere_models 
    /// @param other_models 
    void SphereBoxCollision(const std::vector<model*>& sphere_models, const std::vector<model*>& other_models);


    /// En gammel collsison check som er basert AABB collision (ikke i bruk)
    /// @param AllModels 
    void CollisionCheck(std::vector<model*> AllModels);

  
};
