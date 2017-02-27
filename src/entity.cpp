#include "entity.h"

Entity::Entity() {
    // MALLOC:
    collisionShape_ = new btBoxShape(btVector3(1, 1, 1));
    collisionObject_ = new btCollisionObject();
    bulletTransform_ = new btTransform(btQuaternion(0, 0, 0));
    collisionObject_->setCollisionShape(collisionShape_);
    collisionObject_->setWorldTransform(*bulletTransform_);
}
Entity::~Entity() {
    //delete collisionShape_;
}
void Entity::setMesh(const Mesh* mesh) {
    mesh_ = mesh;
}
const Mesh* Entity::getMesh() const {
    return mesh_;
}
Transform& Entity::getTransform() {
    return transform_;
}
const Transform& Entity::getTransform() const {
    return transform_;
}
void Entity::rotate(const glm::quat& rotation) {
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
void Entity::scale(float scale) {
    transform_.scale(scale);
}
btCollisionObject* Entity::getCollisionObject() {
    //NOTE: Dangerous if memloc changes
    return collisionObject_;
}
void Entity::update() {
    auto o = transform_.getOrientation();
    bulletTransform_->setRotation(btQuaternion(o.x, o.y, o.z, o.w));
    auto p = transform_.getPosition();
    bulletTransform_->setOrigin(btVector3(p.x, p.y, p.z));
    collisionObject_->setWorldTransform(*bulletTransform_);
}