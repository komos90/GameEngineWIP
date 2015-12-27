
#include "common_imports.h"
#include "camera.h"

Camera::Camera() {
    up = glm::vec3(0.f, 1.f, 0.f);
}
const Transform& Camera::getTransform() const {
    return transform;
}
void Camera::rotate(glm::quat rotation) {
    transform.rotate(rotation);
}
void Camera::rotateHorizontal(F32 radians) {
    transform.setOrientation(glm::rotate(transform.getOrientation(), radians, up));
}
void Camera::rotateVertical(F32 radians) {
    transform.setOrientation(glm::rotate(transform.getOrientation(), radians, up));
}
void Camera::translate(const glm::vec3& translation) {
    transform.translate(translation);
}