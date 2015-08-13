
#include <vector>

#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
//#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.h"

bool Renderer::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("SDL Initialisation failed.");
        return false;
    }
    if (SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &window, &renderer) == -1) {
        SDL_Log("Error creating window and renderer.");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == NULL) {
        SDL_Log("glContext is NULL");
        return false;
    }

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        SDL_Log("glewError != GLEW_OK");
        return false;
    }

    if (SDL_GL_SetSwapInterval(1) < 0) {
        SDL_Log("Setting VSynch failed");
        return false;
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shaderProgram.init();
    shaderProgram.loadVertexShader("src/shaders/basic.glvs");
    shaderProgram.loadFragmentShader("src/shaders/basic.glfs");
    shaderProgram.linkProgram();

    vertexPos3DLocation = glGetAttribLocation(shaderProgram.getProgramId(), "LVertexPos3D");
    if (vertexPos3DLocation == -1) {
        SDL_Log("Error getting vertexPos2DLocation");
    }

    vertexNormal = glGetAttribLocation(shaderProgram.getProgramId(), "normals");
    MVP = glGetUniformLocation(shaderProgram.getProgramId(), "MVP");
    lightID = glGetUniformLocation(shaderProgram.getProgramId(), "light");

    return true;
}
void Renderer::render(Entity entity, Camera camera) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float s = 0.5;

    glm::mat4 rotationMatrix = glm::mat4_cast(entity.getTransform().getOrientation());
    glm::mat4 translationMatrix(1.f);
    translationMatrix[3][0] = entity.getTransform().getPosition().x;
    translationMatrix[3][1] = entity.getTransform().getPosition().y;
    translationMatrix[3][2] = entity.getTransform().getPosition().z;

    glm::mat4 cameraRotationMatrix = glm::inverse(glm::mat4_cast(camera.getTransform().getOrientation()));
    glm::mat4 cameraTranslationMatrix(1.f);
    translationMatrix[3][0] = camera.getTransform().getPosition().x;
    translationMatrix[3][1] = camera.getTransform().getPosition().y;
    translationMatrix[3][2] = camera.getTransform().getPosition().z;

    glm::mat4 scaleMatrix(s);
    scaleMatrix[3][3] = 1.f;

    glm::mat4 perspectiveMatrix = glm::perspective(glm::pi<float>() / 2.f, 4.f / 3.f, 0.1f, 500.f);

    glm::mat4 modelViewMatrix = perspectiveMatrix * cameraRotationMatrix * cameraTranslationMatrix * translationMatrix * rotationMatrix * scaleMatrix;

    glm::vec3 testLight(2.f, 0.f, 0.f);

    const std::vector<Vertex>& meshVertices = entity.getMesh()->getVertices();
    std::vector<glm::vec4> meshCopy;
    for each (const Vertex& v in meshVertices) {
        meshCopy.push_back(v.getVector());
    }

    const std::vector<Face>& meshFaces = entity.getMesh()->getFaces();
    GLuint* indexData = new GLuint[meshFaces.size() * 3];
    for (int i = 0; i < (int)meshFaces.size(); i++) {
        indexData[i * 3] = meshFaces[i].getVertexIndexAt(0);
        indexData[i * 3 + 1] = meshFaces[i].getVertexIndexAt(1);
        indexData[i * 3 + 2] = meshFaces[i].getVertexIndexAt(2);
    }

    std::vector<glm::vec3> normals(meshCopy.size());
    for (int i = 0; i < (int)meshCopy.size(); i++) {
        const std::vector<int>& tmp = entity.getMesh()->getVertices()[i].getFaces();
        glm::vec3 sum(0.f, 0.f, 0.f);
        for (int j = 0; j < (int)tmp.size(); j++) {
            sum += entity.getMesh()->getFaces()[tmp[j]].getNormal();
        }
        sum /= tmp.size();
        normals[i] = sum;
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, meshCopy.size() * 4 * sizeof(GLfloat), &meshCopy[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, entity.getMesh()->getFaces().size() * 3 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

    glGenBuffers(1, &VNBO);
    glBindBuffer(GL_ARRAY_BUFFER, VNBO);
    glBufferData(GL_ARRAY_BUFFER, meshCopy.size() * 3 * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);

    //Render

    glUseProgram(shaderProgram.getProgramId());
    glUniformMatrix4fv(MVP, 1, GL_FALSE, &modelViewMatrix[0][0]);
    glUniform3fv(lightID, 1, &testLight[0]);

    glEnableVertexAttribArray(vertexPos3DLocation);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(vertexPos3DLocation, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glEnableVertexAttribArray(vertexNormal);
    glBindBuffer(GL_ARRAY_BUFFER, VNBO);
    glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

    glDrawElements(GL_TRIANGLES, 3 * entity.getMesh()->getFaces().size(), GL_UNSIGNED_INT, NULL);
    glDisableVertexAttribArray(vertexPos3DLocation);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteBuffers(1, &VNBO);
    glUseProgram(NULL);

    SDL_GL_SwapWindow(window);
}