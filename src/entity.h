#pragma once

#include <glm/vec3.hpp>

#include "transform.h"
#include "mesh.h"

class Entity {
private:
    Transform transform_;
    const Mesh* mesh_;
public:
    void setMesh(const Mesh* mesh);
    const Mesh* getMesh();
    const Transform& getTransform() const;
    void rotate(glm::quat rotation);
    void translate(const glm::vec3& translation);
    void setPosition(const glm::vec3& position);
};