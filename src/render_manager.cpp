#include <vector>

#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "render_manager.h"
#include "resource_manager.h"
#include "assertions.h"
#include "debug_draw.h"

// Global singleton instance
RenderManager gRenderManager;


RenderManager::RenderManager() {
    ASSERT(this == &gRenderManager);
}

RenderManager::~RenderManager() {}

void RenderManager::init() {
    int sdlError = SDL_Init(SDL_INIT_EVERYTHING);
    ASSERT(sdlError == 0);
    sdlError = SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &window_, &renderer_);
    ASSERT(sdlError != -1);

    int Mv = 3;
    int mv = 3;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, Mv);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, mv);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    int getMv, getmv;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &getMv);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &getmv);
    ASSERT(getMv == Mv && getmv == mv);
    SDL_GLContext glContext = SDL_GL_CreateContext(window_);
    ASSERT(glContext != NULL);

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    ASSERT(glewError == GLEW_OK);
    // Bug in glew sets error flag.
    glGetError();

    sdlError = SDL_GL_SetSwapInterval(1);
    ASSERT(sdlError >= 0);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    shaderProgram_.init();
    shaderProgram_.loadVertexShader("src/shaders/vertex_shader.glsl");
    shaderProgram_.loadFragmentShader("src/shaders/fragment_shader.glsl");
    shaderProgram_.linkProgram();

    //Load debug lines shader
    debugLinesShader_.init();
    debugLinesShader_.loadVertexShader("src/shaders/debug_line_vertex_shader.glsl");
    debugLinesShader_.loadFragmentShader("src/shaders/debug_line_fragment_shader.glsl");
    debugLinesShader_.linkProgram();

    vertexPos3DHandle_ = glGetAttribLocation(shaderProgram_.getProgramId(), "LVertexPos3D");
    ASSERT(vertexPos3DHandle_ != -1);
    uvTexCoordsHandle_ = glGetAttribLocation(shaderProgram_.getProgramId(), "vertexUV");
    ASSERT(uvTexCoordsHandle_ != -1);

    vertexNormalHandle_ = glGetAttribLocation(shaderProgram_.getProgramId(), "normals");
    textureSamplerHandle_ = glGetUniformLocation(shaderProgram_.getProgramId(), "textureSampler");
    ModelViewProjectionHandle_ = glGetUniformLocation(shaderProgram_.getProgramId(), "MVP");
    ModelWorldProjectionHandle_ = glGetUniformLocation(shaderProgram_.getProgramId(), "MWP");
}

void RenderManager::destroy() {
    SDL_Quit();
}

void RenderManager::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderManager::render(const Entity& entity, const Camera& camera) {
    // NOTE: Should an entity be a POD?????
    Transform entityTransform = entity.getTransform();
    glm::mat4 rotationMatrix = glm::mat4_cast(entityTransform.getOrientation());
    glm::mat4 scaleMatrix(entity.getTransform().getScale());
    scaleMatrix[3][3] = 1.f;
    glm::mat4 translationMatrix(1.f);
    const glm::vec3& entityPosition = entityTransform.getPosition();
    translationMatrix[3][0] = entityPosition.x;
    translationMatrix[3][1] = entityPosition.y;
    translationMatrix[3][2] = entityPosition.z;

    Transform cameraTransform = camera.getTransform();
    glm::mat4 cameraRotationMatrix = glm::mat4_cast(glm::conjugate(cameraTransform.getOrientation()));
    glm::mat4 cameraTranslationMatrix(1.f);
    const glm::vec3& cameraPosition = -cameraTransform.getPosition();
    cameraTranslationMatrix[3][0] = cameraPosition.x;
    cameraTranslationMatrix[3][1] = cameraPosition.y;
    cameraTranslationMatrix[3][2] = cameraPosition.z;

    glm::mat4 perspectiveMatrix = glm::perspective(glm::pi<F32>() / 2.f, 4.f / 3.f, 0.1f, 500.f);

    glm::mat4 modelWorldMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    glm::mat4 modelViewMatrix = perspectiveMatrix * cameraRotationMatrix * cameraTranslationMatrix * modelWorldMatrix;

    glm::vec3 testLight(1.f, 0.f, 0.f);


    // Only has to be done if data not already in GPU
    // NOTE: Encapsulate??
    const Mesh* entityMesh = entity.getMesh();
    ASSERT(entityMesh != nullptr);
    GLuint texId = -1;
    if (true) {
        std::vector<glm::vec4> vertexData;
        std::vector<glm::vec2> uvData;
        std::vector<glm::vec3> normalData;
        for (const auto& face : entityMesh->getFaces()) {
            for (int i = 0; i < 3; i++) {
                vertexData.push_back(entityMesh->getVertices()[face.getVertexIndexAt(i)].getVector());
                uvData.push_back(entityMesh->getTextureCoords()[face.getUvIndexAt(i)]);
                normalData.push_back(entityMesh->getVertices()[face.getVertexIndexAt(i)].getNormal());
            }
        }

        // Create & fill buffers if they haven't already been created
        if (true) {
            glGenBuffers(1, &vertexBufferHandle_);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle_);
            glBufferData(GL_ARRAY_BUFFER, vertexData.size() * 4 * sizeof(GLfloat), &vertexData[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            entityMesh->vboId_ = vertexBufferHandle_;
        }
        if (true) {
            glGenBuffers(1, &textureCoordsBufferHandle_);
            glBindBuffer(GL_ARRAY_BUFFER, textureCoordsBufferHandle_);
            glBufferData(GL_ARRAY_BUFFER, uvData.size() * 2 * sizeof(GLfloat), &uvData[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            entityMesh->uvboId_ = textureCoordsBufferHandle_;
        }
        if (true) {
            glGenBuffers(1, &normalBufferHandle_);
            glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle_);
            glBufferData(GL_ARRAY_BUFFER, normalData.size() * 3 * sizeof(GLfloat), &normalData[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            entityMesh->vnboId_ = normalBufferHandle_;
        }

        //Load Texture
        if (entityMesh->texId_ == -1) {
            const Texture& texRef = entityMesh->getTexture();
            const SDL_Surface* texSurf = texRef.getTexture();
            glGenTextures(1, &texId);
            glBindTexture(GL_TEXTURE_2D, texId);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSurf->w, texSurf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texSurf->pixels);
            GLuint errCode = glGetError();
            if (errCode != GL_NO_ERROR) {
                printf("OpenGL Error Code: %u", errCode);
                ASSERT(false);
            }
            entityMesh->texId_ = texId;
        }
        texId = entityMesh->texId_;
        

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, texId);
    }

    // Render
    glUseProgram(shaderProgram_.getProgramId());
    glUniformMatrix4fv(ModelViewProjectionHandle_, 1, GL_FALSE, &modelViewMatrix[0][0]);
    glUniformMatrix4fv(ModelWorldProjectionHandle_, 1, GL_FALSE, &modelWorldMatrix[0][0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glUniform1i(textureSamplerHandle_, 0);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glEnableVertexAttribArray(uvTexCoordsHandle_);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordsBufferHandle_);
    glVertexAttribPointer(uvTexCoordsHandle_, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(vertexPos3DHandle_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle_);
    glVertexAttribPointer(vertexPos3DHandle_, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(vertexNormalHandle_);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle_);
    glVertexAttribPointer(vertexNormalHandle_, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glDrawArrays(GL_TRIANGLES, 0, 3 * entity.getMesh()->getFaces().size());
    glDisableVertexAttribArray(vertexPos3DHandle_);
    // NOTE: When should I glDeleteBuffer() ??
    glUseProgram(NULL);
}

void RenderManager::renderDebug(DebugDraw& dDraw, const Camera& camera) {
    Transform cameraTransform = camera.getTransform();
    glm::mat4 cameraRotationMatrix = glm::mat4_cast(glm::conjugate(cameraTransform.getOrientation()));
    glm::mat4 cameraTranslationMatrix(1.f);
    const glm::vec3& cameraPosition = -cameraTransform.getPosition();
    cameraTranslationMatrix[3][0] = cameraPosition.x;
    cameraTranslationMatrix[3][1] = cameraPosition.y;
    cameraTranslationMatrix[3][2] = cameraPosition.z;

    glm::mat4 perspectiveMatrix = glm::perspective(glm::pi<F32>() / 2.f, 4.f / 3.f, 0.1f, 500.f);
    glm::mat4 modelViewMatrix = perspectiveMatrix * cameraRotationMatrix * cameraTranslationMatrix;
    
    auto vertexPos3DHandle = glGetAttribLocation(debugLinesShader_.getProgramId(), "LVertexPos3D");
    auto lineColorHandle = glGetAttribLocation(debugLinesShader_.getProgramId(), "lineColor");
    auto ModelViewProjectionHandle = glGetUniformLocation(debugLinesShader_.getProgramId(), "MVP");

    glDisable(GL_DEPTH_TEST);
    glUseProgram(debugLinesShader_.getProgramId());
    while (!dDraw.lines_.empty()) {
        GLuint lineVBO;
        auto dLine = dDraw.popDebugLine();
        GLfloat lineBuf[] = {dLine.start_.x, dLine.start_.y, dLine.start_.z,
                             dLine.color_.r, dLine.color_.g, dLine.color_.b,
                             dLine.end_.x, dLine.end_.y, dLine.end_.z,
                             dLine.color_.r, dLine.color_.g, dLine.color_.b};
        glUniformMatrix4fv(ModelViewProjectionHandle, 1, GL_FALSE, &modelViewMatrix[0][0]);

        glGenBuffers(1, &lineVBO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), &lineBuf[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(vertexPos3DHandle);
        glVertexAttribPointer(vertexPos3DHandle, 3, GL_FLOAT, GL_FALSE, 2 * 3 * sizeof(float), 0);

        glEnableVertexAttribArray(lineColorHandle);
        glVertexAttribPointer(lineColorHandle, 3, GL_FLOAT, GL_FALSE, 2 * 3 * sizeof(float), (GLvoid*) (1 * 3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_LINES, 0, 2);
    }
    glUseProgram(NULL);
    glEnable(GL_DEPTH_TEST);
    dDraw.flushLines();
}

void RenderManager::swap() {
    SDL_GL_SwapWindow(window_);
}