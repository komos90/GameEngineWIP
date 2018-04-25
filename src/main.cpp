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

#include "common_imports.h"
#include "mesh.h"
#include "game.h"
#include "render_manager.h"
#include "file_system_manager.h"
#include "resource_manager.h"

// Debug memory leaks
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

//Globals
const S32 FPS_CAP = 60;

/*------------------------------------------
 * There are 39 000 Lines of Code in Doom.
 -----------------------------------------*/
int main(S32 argc, char* argv[]) {
    // Initialise Subsystems
    gFileSystemManager.init();
    gResourceManager.init();
    gRenderManager.init();

    // NOTE: Should this be here or in gResourceManager.init() or in GameStateStack?
    gResourceManager.loadGlobalResources();
    Game game;
    bool running = true;
    while (running) {
        //Manage fps
        S32 frameStartTime = SDL_GetTicks();

        game.events();
        game.logic();
        game.draw();
        running = game.isRunning();

        //Manage fps
        int frameDelta = SDL_GetTicks() - frameStartTime;
        if (frameDelta < 1000 / FPS_CAP) {
            SDL_Delay(1000 / FPS_CAP - frameDelta);
        }
    }

    //Shutdown subsystems
    gRenderManager.destroy();
    gResourceManager.init();
    gFileSystemManager.destroy();

    //_CrtDumpMemoryLeaks();
	return 0;
}