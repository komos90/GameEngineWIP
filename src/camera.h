#pragma once

#include <glm/vec3.hpp>
#include <glm\gtc\quaternion.hpp>

#include "common_imports.h"
#include "transform.h"

class Camera {
private:
    Transform transform_;
    glm::vec3 up_;
public:
    explicit Camera();
    void rotate(const glm::quat& rotation);
    void rotateHorizontal(F32 radians);
    void rotateVertical(F32 radians);
    void translate(const glm::vec3& translation);
    Transform& getTransform();
    const Transform& getTransform() const;
};