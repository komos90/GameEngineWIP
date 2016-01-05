#include <SDL.h>


#include "common_imports.h"
#include "game.h"
#include "render_manager.h"
#include "resource_manager.h"
#include "level.h"

Game::Game() :
    level_("level1")
{
    
}

Game::~Game() {

}

bool Game::isRunning() {
    return running_;
}
void Game::events() {
    level_.events();
}
void Game::logic() {
    level_.logic();
}
void Game::draw() const {
    gRenderManager.clear();
    level_.draw();
    gRenderManager.swap();
}