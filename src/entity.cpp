#include "entity.h"

void Entity::setMesh(const Mesh* mesh) {
    mesh_ = mesh;
}
const Mesh* Entity::getMesh() {
    return mesh_;
}
const Transform& Entity::getTransform() const {
    return transform_;
}
void Entity::rotate(glm::quat rotation) {
    transform_.rotate(rotation);
}
void Entity::translate(const glm::vec3& translation) {
    transform_.translate(translation);
}
void Entity::setPosition(const glm::vec3& position) {
    transform_.setPosition(position);
}
void Entity::addPosition(const glm::vec3& position) {
    transform_.translate(position);
}