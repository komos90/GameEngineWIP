
#include "common_imports.h"
#include "camera.h"

Camera::Camera() {
    up_ = glm::vec3(0.f, 1.f, 0.f);
}
Transform& Camera::getTransform() {
    return transform_;
}
const Transform& Camera::getTransform() const {
    return transform_;
}
void Camera::rotate(const glm::quat& rotation) {
    transform_.rotate(rotation);
}
void Camera::rotateHorizontal(F32 radians) {
    transform_.setOrientation(glm::rotate(transform_.getOrientation(), radians, up_));
}
void Camera::rotateVertical(F32 radians) {
    transform_.setOrientation(glm::rotate(transform_.getOrientation(), radians, up_));
}
void Camera::translate(const glm::vec3& translation) {
    transform_.translate(translation);
}