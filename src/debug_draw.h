#pragma once

#include <queue>
#include <memory>

#include <glm/vec3.hpp>

#include "btBulletDynamicsCommon.h"

class DebugLine {
public:
    glm::vec3 start_;
    glm::vec3 end_;
    glm::vec3 color_;

    DebugLine(glm::vec3 start, glm::vec3 end, glm::vec3 color);

};

class DebugDraw : public btIDebugDraw {
public:
    std::queue<DebugLine> lines_;  
    void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
    void	drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
    void	reportErrorWarning(const char* warningString) override;
    void	draw3dText(const btVector3& location, const char* textString) override;
    void	setDebugMode(int debugMode) override;
    int		getDebugMode() const override;
    void    flushLines() override;
    DebugLine popDebugLine();
};