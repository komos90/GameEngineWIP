#pragma once

#include "camera.h"
#include "mesh.h"
#include "entity.h"

class Game {
private:
    // 
    // GameStateStack gameStates;
    // tmp
    Camera camera;
    const Mesh* test;
    Entity testMonkey1;
    // end tmp

    bool running = true;

public:
    explicit Game();
    ~Game();
    bool isRunning();
    void events();
    void logic();
    void draw() const;
};