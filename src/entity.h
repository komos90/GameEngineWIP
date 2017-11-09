#pragma once

#include <glm/vec3.hpp>

#include "btBulletDynamicsCommon.h"

#include <memory>

#include "transform.h"
#include "mesh.h"

class Entity {
private:
    Transform transform_;
    const Mesh* mesh_;
    std::unique_ptr<btCollisionObject> collisionObject_;
    std::unique_ptr<btCollisionShape> collisionShape_;
    btTransform bulletTransform_;
public:
    explicit Entity();
    ~Entity();
    Entity(Entity &&) = default;
    void Entity::setupCollisionObject();
    void setMesh(const Mesh* mesh);
    const Mesh* getMesh() const;
    Transform& getTransform();
    const Transform& getTransform() const;
    void rotate(const glm::quat& rotation);
    void scale(float scale);
    void translate(const glm::vec3& translation);
    void setPosition(const glm::vec3& position);
    void addPosition(const glm::vec3& position);
    btCollisionObject* getCollisionObject();
    void update();
};