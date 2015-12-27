
#include "common_imports.h"
#include "game.h"

bool Game::init() {
    if (!renderer.init()) {
        SDL_Log("Renderer initialisation failed.");
        return false;
    }
    //tmp
    test.daeFileToMesh("res/meshes/crate.dae");
    //levelTerrainMesh.daeFileToMesh("res/meshes/level1terrain.raw");

    testMonkey1.setMesh(&test);
    testMonkey1.translate(glm::vec3(0.f, 0.f, -5.f));
    //testMonkey2.setMesh(&test);
    //levelTerrain.setMesh(&levelTerrainMesh);
    //testMonkey2.translate(glm::vec3(0.5f, 0.5f, 0.5f));
    return true;
}
bool Game::isRunning() {
    return running;
}
void Game::events() {
    //Events
    SDL_Event e;
    while (SDL_PollEvent(&e) == 1) {
        switch (e.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym) {
            case SDLK_w:
                camera.translate(glm::vec3(0.f, 0.f, 0.1f));
                break;
            case SDLK_s:
                camera.translate(glm::vec3(0.f, 0.f, -0.1f));
                break;
            case SDLK_a:
                camera.translate(glm::vec3(0.1f, 0.f, 0.f));
                break;
            case SDLK_d:
                camera.translate(glm::vec3(-0.1f, 0.f, 0.f));
                break;
            case SDLK_ESCAPE:
                running = false;
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            camera.rotate(glm::quat(glm::vec3(-(F32)e.motion.yrel / 400.f, -(F32)e.motion.xrel / 400.f, 0.f)));
            break;
        }
    }
}
void Game::logic() {
    testMonkey1.rotate(glm::quat(glm::vec3(0.03f, 0.03f, 0.03f)));
    //testMonkey2.rotate(glm::quat(glm::vec3(-0.05f, 0.05f, 0.05f)));
}
void Game::draw() const {

    renderer.render(testMonkey1, camera);
    //renderer.render(levelTerrain, camera);
    //renderer.render(testMonkey2);
}