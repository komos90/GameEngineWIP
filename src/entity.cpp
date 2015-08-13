#include "entity.h"

void Entity::setMesh(const Mesh* mesh) {
    this->mesh = mesh;
}
const Mesh* Entity::getMesh() {
    return mesh;
}
const Transform& Entity::getTransform() const {
    return transform;
}
void Entity::rotate(glm::quat rotation) {
    transform.rotate(rotation);
}
void Entity::translate(const glm::vec3& translation) {
    transform.translate(translation);
}
