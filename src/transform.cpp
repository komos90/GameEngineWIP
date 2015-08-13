#include "transform.h"

const glm::vec3& Transform::getPosition() const {
    return position;
}
const glm::quat& Transform::getOrientation() const {
    return orientation;
}
void Transform::setOrientation(const glm::quat& orientation) {
    this->orientation = orientation;
}
void Transform::rotate(glm::quat rotation) {
    orientation *= rotation;
}
void Transform::translate(const glm::vec3& translation) {
    position += translation;
}