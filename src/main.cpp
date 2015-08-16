#include <vector>
#include <fstream>

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>
#include <glm/geometric.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>


#include "mesh.h"
#include "game.h"

//Globals
const int FPS_CAP = 60;


int main(int argc, char* argv[]) {

    Game game;
    if (!game.init()) {
        SDL_Log("Game initialisation failed.");
    }

    float theta = 0.01f;
    bool running = true;
    while (running) {
        //Manage fps
        int frameStartTime = SDL_GetTicks();

        game.events();
        game.logic();
        game.draw();

        if (!game.isRunning()) running = false;
        //Manage fps
        int frameDelta = SDL_GetTicks() - frameStartTime;
        if (frameDelta < 1000 / FPS_CAP) {
            SDL_Delay(1000 / FPS_CAP - frameDelta);
        }
    }
	return 0;
}