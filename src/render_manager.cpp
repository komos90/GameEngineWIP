#include <vector>
#include <array>

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
#include "prof_timer.h"
#include "time_manager.h"

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaderProgram_.init();
    shaderProgram_.loadVertexShader("src/shaders/vertex_shader.glsl");
    shaderProgram_.loadFragmentShader("src/shaders/fragment_shader.glsl");
    shaderProgram_.linkProgram();

    //Load debug lines shader
    debugLinesShader_.init();
    debugLinesShader_.loadVertexShader("src/shaders/debug_line_vertex_shader.glsl");
    debugLinesShader_.loadFragmentShader("src/shaders/debug_line_fragment_shader.glsl");
    debugLinesShader_.linkProgram();

    waterShader_.init();
    waterShader_.loadVertexShader("src/shaders/water_vertex_shader.glsl");
    waterShader_.loadGeometryShader("src/shaders/water_geometry_shader.glsl");
    waterShader_.loadFragmentShader("src/shaders/untextured_fragment_shader.glsl");
    waterShader_.linkProgram();

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
    //timer.start();
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

    const Mesh* entityMesh = entity.getMesh();
    ASSERT(entityMesh != nullptr);

    // Create & fill buffers if they haven't already been created
    if (entityMesh->vboId_ == -1) {
        const auto& vertexData = entityMesh->vertexData();
        glGenBuffers(1, &entityMesh->vboId_);
        glBindBuffer(GL_ARRAY_BUFFER, entityMesh->vboId_);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * 4 * sizeof(GLfloat), &vertexData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (entityMesh->uvboId_ == -1) {
        const auto& uvData = entityMesh->uvData();
        glGenBuffers(1, &entityMesh->uvboId_);
        glBindBuffer(GL_ARRAY_BUFFER, entityMesh->uvboId_);
        glBufferData(GL_ARRAY_BUFFER, uvData.size() * 2 * sizeof(GLfloat), &uvData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (entityMesh->vnboId_ == -1) {
        const auto& normalData = entityMesh->normalData();
        glGenBuffers(1, &entityMesh->vnboId_);
        glBindBuffer(GL_ARRAY_BUFFER, entityMesh->vnboId_);
        glBufferData(GL_ARRAY_BUFFER, normalData.size() * 3 * sizeof(GLfloat), &normalData[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    //Load Texture
    GLuint texId = -1;
    if (entityMesh->texId_ == -1) {
        const Texture& texRef = entityMesh->getTexture();
        const SDL_Surface* texSurf = texRef.getTexture();
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSurf->w, texSurf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texSurf->pixels);
        GLuint errCode = glGetError();
        if (errCode != GL_NO_ERROR) {
            printf("OpenGL Error Code: %u\n", errCode);
            ASSERT(false);
        }
        entityMesh->texId_ = texId;
    }
    texId = entityMesh->texId_;
        

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, texId);

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
    glBindBuffer(GL_ARRAY_BUFFER, entityMesh->uvboId_);
    glVertexAttribPointer(uvTexCoordsHandle_, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(vertexPos3DHandle_);
    glBindBuffer(GL_ARRAY_BUFFER, entityMesh->vboId_);
    glVertexAttribPointer(vertexPos3DHandle_, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(vertexNormalHandle_);
    glBindBuffer(GL_ARRAY_BUFFER, entityMesh->vnboId_);
    glVertexAttribPointer(vertexNormalHandle_, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 3 * entity.getMesh()->getFaces().size());
    glDisableVertexAttribArray(vertexPos3DHandle_);
    // NOTE: When should I glDeleteBuffer() ??
    glUseProgram(NULL);
    //timer.stop();
}

template <size_t N>
std::array<glm::vec4, N*N*6> createPlaneMesh(F32 zOff, F32 scale) {
    const int vertsInSquare = 6;
    scale *= 2;
    std::array<glm::vec4, N*N*6> vertices;
    for (int y = 0; y < N; ++y) {
        for (int x = 0; x < N; ++x) {
            F32 invN = 1.f / N;
            F32 xOff = x * invN;
            F32 yOff = y * invN;
            size_t arrayOff = (y * N + x) * 6;
            vertices[arrayOff    ] = glm::vec4{ (xOff        - 0.5f) * scale, (yOff        - 0.5f) * scale, zOff, 1.f };
            vertices[arrayOff + 1] = glm::vec4{ (xOff + invN - 0.5f) * scale, (yOff        - 0.5f) * scale, zOff, 1.f };
            vertices[arrayOff + 2] = glm::vec4{ (xOff        - 0.5f) * scale, (yOff + invN - 0.5f) * scale, zOff, 1.f };
            vertices[arrayOff + 3] = glm::vec4{ (xOff + invN - 0.5f) * scale, (yOff        - 0.5f) * scale, zOff, 1.f };
            vertices[arrayOff + 4] = glm::vec4{ (xOff + invN - 0.5f) * scale, (yOff + invN - 0.5f) * scale, zOff, 1.f };
            vertices[arrayOff + 5] = glm::vec4{ (xOff        - 0.5f) * scale, (yOff + invN - 0.5f) * scale, zOff, 1.f };
        }
    }
    return vertices;
}

void RenderManager::renderWaterLevel(const Camera& camera) {
    auto planeVertices = createPlaneMesh<10>(0.f, 100.f);

    Transform cameraTransform = camera.getTransform();
    glm::mat4 cameraRotationMatrix = glm::mat4_cast(glm::conjugate(cameraTransform.getOrientation()));
    glm::mat4 cameraTranslationMatrix(1.f);
    const glm::vec3& cameraPosition = -cameraTransform.getPosition();
    cameraTranslationMatrix[3][0] = cameraPosition.x;
    cameraTranslationMatrix[3][1] = cameraPosition.y;
    cameraTranslationMatrix[3][2] = cameraPosition.z;

    glm::mat4 perspectiveMatrix = glm::perspective(glm::pi<F32>() / 2.f, 4.f / 3.f, 0.1f, 500.f);
    glm::mat4 modelViewMatrix = perspectiveMatrix * cameraRotationMatrix * cameraTranslationMatrix;

    // Create & fill buffers if they haven't already been created
    // REMEMBER TO DELETE BUFFER
    GLuint vboId = -1;
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, planeVertices.size() * 4 * sizeof(GLfloat), &planeVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(waterShader_.getProgramId());

    //No need to call uniform every frame!
    GLint timeHandle = glGetUniformLocation(waterShader_.getProgramId(), "time");
    float time = (float)gTimeManager.getTicks() / (float)gTimeManager.getTicksPerSecond() + 0.5f;
    printf("%f\n", fmod(time, (2 * 3.14159265359)));
    glUniform1f(timeHandle, fmod(time, (2 * 3.14159265359)));
    GLuint errCode = glGetError();
    if (errCode != GL_NO_ERROR) {
        printf("OpenGL Error Code: %u\n", errCode);
        ASSERT(false);
    }

    GLuint mvpHandle = glGetUniformLocation(waterShader_.getProgramId(), "MVP");
    glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &modelViewMatrix[0][0]);

    errCode = glGetError();
    if (errCode != GL_NO_ERROR) {
        printf("OpenGL Error Code: %u\n", errCode);
        ASSERT(false);
    }
    
    GLuint VertexArrayID;
    //SHOULD DELETE VERTEX ARRAY
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint vertexPos3DHandle = glGetAttribLocation(waterShader_.getProgramId(), "LVertexPos3D");
    glEnableVertexAttribArray(vertexPos3DHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glVertexAttribPointer(vertexPos3DHandle, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, planeVertices.size());
    glDisableVertexAttribArray(vertexPos3DHandle);
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
    
    const size_t lineBufferSize = 4 * 3 * sizeof(GLfloat);

    for (auto lineVBO : debugLineHandles_) {
        glDeleteBuffers(1, &lineVBO);
    }
    debugLineHandles_.clear();
    while (!dDraw.lines_.empty()) {
        GLuint lineVBO = -1;
        auto dLine = dDraw.popDebugLine();
        GLfloat lineBuf[] = {dLine.start_.x, dLine.start_.y, dLine.start_.z,
                             dLine.color_.r, dLine.color_.g, dLine.color_.b,
                             dLine.end_.x, dLine.end_.y, dLine.end_.z,
                             dLine.color_.r, dLine.color_.g, dLine.color_.b};
        glUniformMatrix4fv(ModelViewProjectionHandle, 1, GL_FALSE, &modelViewMatrix[0][0]);

        glGenBuffers(1, &lineVBO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, lineBufferSize, &lineBuf[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(vertexPos3DHandle);
        glVertexAttribPointer(vertexPos3DHandle, 3, GL_FLOAT, GL_FALSE, 2 * 3 * sizeof(float), 0);

        glEnableVertexAttribArray(lineColorHandle);
        glVertexAttribPointer(lineColorHandle, 3, GL_FLOAT, GL_FALSE, 2 * 3 * sizeof(float), (GLvoid*) (1 * 3 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_LINES, 0, 2);
        debugLineHandles_.push_back(lineVBO);
    }
    glUseProgram(NULL);
    glEnable(GL_DEPTH_TEST);
    dDraw.flushLines();
}

void RenderManager::swap() {
    SDL_GL_SwapWindow(window_);
}