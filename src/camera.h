#pragma once

#include <glm/vec3.hpp>
#include <glm\gtc\quaternion.hpp>

#include "transform.h"

class Camera {
private:
    Transform transform;
    glm::vec3 up;
public:
    explicit Camera();
    void rotate(glm::quat rotation);
    void rotateHorizontal(float radians);
    void rotateVertical(float radians);
    void translate(const glm::vec3& translation);
    const Transform& getTransform() const;
};