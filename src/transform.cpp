#include "transform.h"

const glm::vec3& Transform::getPosition() const {
    return position_;
}
const glm::quat& Transform::getOrientation() const {
    return orientation_;
}
float Transform::getScale() const {
    return scale_;
}
void Transform::scale(float scale) {
    scale_ *= scale;
}
void Transform::setScale(float scale) {
    scale_ = scale;
}
void Transform::setOrientation(const glm::quat& orientation) {
    orientation_ = orientation;
}
void Transform::rotate(const glm::quat& rotation) {
    orientation_ *= rotation;
}
void Transform::translate(const glm::vec3& translation) {
    position_ += translation;
}
void Transform::setPosition(const glm::vec3& position) {
    position_ = position;
}