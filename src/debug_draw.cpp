#include "debug_draw.h"
#include "util.h"

DebugLine::DebugLine(glm::vec3 start, glm::vec3 end, glm::vec3 color):
    start_(start),
    end_(end),
    color_(color)
{}

void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    lines_.push(DebugLine(util::bulletVecToGlmVec(from), util::bulletVecToGlmVec(to), util::bulletVecToGlmVec(color)));
}
void DebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {

}
void DebugDraw::reportErrorWarning(const char* warningString) {

}
void DebugDraw::draw3dText(const btVector3& location, const char* textString) {

}
void DebugDraw::setDebugMode(int debugMode) {

}
int DebugDraw::getDebugMode() const {
    return btIDebugDraw::DBG_DrawWireframe;
}
void DebugDraw::flushLines() {
    std::queue<DebugLine>().swap(lines_);
}
DebugLine DebugDraw::popDebugLine() {
    DebugLine retVal = lines_.front();
    lines_.pop();
    return retVal;
}