#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
private:
    glm::vec3 position_;
    glm::quat orientation_;
    float scale_ = 1.f;
public:
    void scale(float scale);
    void rotate(const glm::quat& rotation);
    void translate(const glm::vec3& translation);
    void setPosition(const glm::vec3& translation);
    void setOrientation(const glm::quat& orientation);
    void setScale(float scale);
    const glm::vec3& getPosition() const;
    const glm::quat& getOrientation() const;
    float getScale() const;
};
