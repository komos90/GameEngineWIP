#pragma once

#include "renderer.h";
#include "camera.h";
#include "mesh.h";
#include "entity.h";

class Game {
private:
    mutable Renderer renderer;
    //tmp
    Camera camera;
    Mesh levelTerrainMesh;
    Mesh test;
    Entity testMonkey1;
    Entity testMonkey2;
    Entity levelTerrain;
    bool running = true;

public:
    bool init();
    bool isRunning();
    void events();
    void logic();
    void draw() const;
};