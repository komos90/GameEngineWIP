#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>

//Globals
const int FPS_CAP = 60;

bool initGl(GLuint* program, GLint* vertexPos2DLocation, GLuint* VBO, GLuint* IBO) {
	bool success = true;

	*program = glCreateProgram();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexShaderSource[] =
	{
		 "#version 140\n\
		  in vec2 LVertexPos2D;\n\
          void main() {\n\
              gl_Position = vec4(LVertexPos2D.x, LVertexPos2D.y, 0, 1 );\n\
          }"
	};
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint didShaderCompile = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &didShaderCompile);
    if (didShaderCompile != GL_TRUE) {
        SDL_Log("Vertex shader compilation error.");
        return false;
    }

    glAttachShader(*program, vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragmentShaderSource[] = 
    {
         "#version 140\n\
         out vec4 LFragment;\n\
         void main() {\n\
             LFragment = vec4( 0.0, 0.5, 0.5, 1.0 );\n\
         }"
    };
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &didShaderCompile);
    if (didShaderCompile != GL_TRUE) {
        SDL_Log("Fragment shader failed to compile.");
        return false;
    }

    glAttachShader(*program, fragmentShader);
    glLinkProgram(*program);
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(*program, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE) {
        SDL_Log("Error linking program");
        return false;
    }

    *vertexPos2DLocation = glGetAttribLocation(*program, "LVertexPos2D");
    if (*vertexPos2DLocation == -1) {
        SDL_Log("Error getting vertexPos2DLocation");
        return false;
    }

    glClearColor(0.f, 0.f, 0.f, 1.f);
    GLfloat vertexData[] = {
         -0.5f, -0.5f,
          0.5f, -0.5f,
          0.5f,  0.5f,
         -0.5f,  0.5f
    };
    GLuint indexData[] = {0, 1, 2 ,3};

    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
    return true;
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window;
    SDL_Renderer* renderer;
	SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &window, &renderer);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL) {
		SDL_Log("glContext is NULL");
		exit(1);
	}

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		SDL_Log("glewError != GLEW_OK");
		exit(1);
	}

	if (SDL_GL_SetSwapInterval(1) < 0) {
		SDL_Log("Setting VSynch failed");
		exit(1);
	}

    GLuint program = 0;
    GLint vertexPos2DLocation = -1;
    GLuint VBO = 0;
    GLuint IBO = 0;
	if (!initGl(&program, &vertexPos2DLocation, &VBO, &IBO)) {
		SDL_Log("initGL failed.");
		exit(1);
	}

    bool running = true;
    while (running) {
        //Manage fps
        int frameStartTime = SDL_GetTicks();

        //Events
        SDL_Event e;
        while (SDL_PollEvent(&e) == 1) {
            switch (e.type) {
            case SDL_QUIT:
                running = false;
            }
        }

        //Logic

        //Render
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glEnableVertexAttribArray(vertexPos2DLocation);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(vertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
        glDisableVertexAttribArray(vertexPos2DLocation);
        glUseProgram(NULL);

        SDL_GL_SwapWindow(window);

        //Manage fps
        int frameDelta = SDL_GetTicks() - frameStartTime;
        if (frameDelta < 1000 / FPS_CAP) {
            SDL_Delay(1000 / FPS_CAP - frameDelta);
        }
    }

	return 0;
}