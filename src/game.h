#pragma once

#include "camera.h"
#include "mesh.h"
#include "entity.h"

class Game {
private:
    //tmp
    Camera camera;
    //Mesh levelTerrainMesh;
    const Mesh* test;
    Entity testMonkey1;
    //Entity testMonkey2;
    //Entity levelTerrain;
    bool running = true;

public:
    explicit Game();
    ~Game();
    bool isRunning();
    void events();
    void logic();
    void draw() const;
};