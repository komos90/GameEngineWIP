#pragma once

#include "camera.h"
#include "mesh.h"
#include "entity.h"
#include "level.h"

class Game {
private:
    // 
    // GameStateStack gameStates;
    // tmp
    Level level_;
    // end tmp

    bool running_ = true;

public:
    explicit Game();
    ~Game();
    bool isRunning();
    void events();
    void logic();
    void draw() const;
};