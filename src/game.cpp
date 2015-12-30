#include <SDL.h>


#include "common_imports.h"
#include "game.h"
#include "render_manager.h"
#include "resource_manager.h"
#include "level.h"

Game::Game() {
    // NOTE: Should this be here or in gResourceManager.init()?
    gResourceManager.loadGlobalResources();
    // NOTE: guid path should not have res/engine_ready/
    test = gResourceManager.getMesh("crate/crate.dae");

    //Should create a factory method for Entity. static method in subClass?
    testMonkey1.setMesh(test);
    testMonkey1.translate(glm::vec3(0.f, 0.f, -5.f));
    Level level;
}

Game::~Game() {

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
    testMonkey1.rotate(glm::quat(glm::vec3(0.00f, 0.03f, 0.00f)));
}
void Game::draw() const {

    gRenderManager.render(testMonkey1, camera);
}