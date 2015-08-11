#include <vector>
#include <fstream>

#include <SDL.h>
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/glu.h>
#include <glm/geometric.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <boost\tokenizer.hpp>

//Globals
const int FPS_CAP = 60;

std::string readFileToString(const std::string& filePath) {
    std::ifstream in(filePath, std::ios::in | std::ios::binary);
    if (!in) {
        SDL_Log("Error reading file to string.");
        return "";
    }
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return contents;
}

class ShaderProgram {
private:
    GLuint programId;
public:
    explicit ShaderProgram();
    void loadVertexShader(const std::string& filePath);
    void loadGeometryShader(const std::string& filePath);
    void loadFragmentShader(const std::string& filePath);
    void linkProgram();
    const GLuint getProgramId() const;
};
ShaderProgram::ShaderProgram() {
    programId = glCreateProgram();
}
void ShaderProgram::loadVertexShader(const std::string& filePath) {
    std::string source = readFileToString(filePath);
    const char* cStrSource = source.c_str();
    GLuint shaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderId, 1, &cStrSource, NULL);
    glCompileShader(shaderId);
    GLint didShaderCompile = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &didShaderCompile);
    if (didShaderCompile != GL_TRUE) {
        SDL_Log("Vertex shader compilation error.");
    }
    glAttachShader(programId, shaderId);
}
void ShaderProgram::loadFragmentShader(const std::string& filePath) {
    std::string source = readFileToString(filePath);
    const char* cStrSource = source.c_str();
    GLuint shaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderId, 1, &cStrSource, NULL);
    glCompileShader(shaderId);
    GLint didShaderCompile = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &didShaderCompile);
    if (didShaderCompile != GL_TRUE) {
        SDL_Log("Fragment shader compilation error.");
    }
    glAttachShader(programId, shaderId);
}
void ShaderProgram::linkProgram() {
    glLinkProgram(programId);
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE) {
        SDL_Log("Error linking program");
    }
}
const GLuint ShaderProgram::getProgramId() const {
    return programId;
}

class Face {
private:
    int vertexIndices[3];
    glm::vec3 normal;
public:
    explicit Face(int a, int b, int c);
    void setNormal(glm::vec3 normal);
    int Face::getVertexIndexAt(int i) const;
    const glm::vec3& Face::getNormal() const;
};
Face::Face(int a, int b, int c) {
    vertexIndices[0] = a;
    vertexIndices[1] = b;
    vertexIndices[2] = c;
}
void Face::setNormal(glm::vec3 normal) {
    this->normal = normal;
}
int Face::getVertexIndexAt(int i) const {
    return vertexIndices[i];
}
const glm::vec3& Face::getNormal() const {
    return normal;
}

class Vertex {
private:
    glm::vec4 vertex;
    std::vector<int> faces;
public:
    explicit Vertex(glm::vec4 &vector);
    void addFaceIndex(int i);
    const glm::vec4& getVector() const;
    const std::vector<int>& getFaces() const;
};
void Vertex::addFaceIndex(int i) {
    faces.push_back(i);
}
Vertex::Vertex(glm::vec4 &vector) {
    vertex = vector;
}
const glm::vec4& Vertex::getVector() const {
    return vertex;
}
const std::vector<int>& Vertex::getFaces() const {
    return faces;
}

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
public:
    void rawFileToMesh(std::string filePath);
    const std::vector<Vertex>& getVertices() const;
    const std::vector<Face>& getFaces() const;
};
const std::vector<Vertex>& Mesh::getVertices() const {
    return vertices;
}
const std::vector<Face>& Mesh::getFaces() const {
    return faces;
}


//TEMPORARY MESH FILE FORMAT
void Mesh::rawFileToMesh(std::string filePath) {
    char buf[512];
    std::fstream fs = std::fstream(filePath, std::ios_base::in);
    fs.getline(buf, 512);
    
        glm::vec4 vertices[3];
        int vertexIndices[3];
        std::string strBuf(buf);
        boost::char_separator<char> sep(" ");
    while (!fs.eof()) {
        
        strBuf = buf;
        boost::tokenizer<boost::char_separator<char> > tokens(strBuf, sep);

        int i = 0;
        for (const auto& t : tokens) {
            switch (i % 3)
            {
            case 0:
                vertices[i / 3].x = std::stof(t);
                break;
            case 1:
                vertices[i / 3].y = std::stof(t);
                break;
            case 2:
                vertices[i / 3].z = std::stof(t);
                vertices[i / 3].w = 1.f;
                
                vertexIndices[i / 3] = -1;
                for (int j = 0; j < this->vertices.size(); j++) {
                    if (this->vertices.at(j).getVector() == vertices[i / 3]) {
                        vertexIndices[i / 3] = j;
                        break;
                    }
                }
                if (vertexIndices[i / 3] == -1) {
                    this->vertices.push_back(Vertex(vertices[i / 3]));
                    vertexIndices[i / 3] = this->vertices.size() - 1;
                }
                this->vertices.at(vertexIndices[i/3]).addFaceIndex(faces.size());
                break;
            }
            i++;
        }
        int s = this->vertices.size() - 1;
        faces.push_back(Face(vertexIndices[0], vertexIndices[1], vertexIndices[2]));

        faces.back().setNormal(glm::normalize(glm::cross(glm::vec3(this->vertices[vertexIndices[0]].getVector() - this->vertices[vertexIndices[1]].getVector()), glm::vec3(this->vertices[vertexIndices[0]].getVector() - this->vertices[vertexIndices[2]].getVector()))));

        fs.getline(buf, 512);
    }
}

bool initGl() {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
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

    GLint vertexPos3DLocation = -1;
    GLuint VBO = 0;
    GLuint IBO = 0;
    GLuint VNBO = 0;
    GLuint INBO = 0;
	if (!initGl()) {
		SDL_Log("initGL failed.");
		exit(1);
	}
    ShaderProgram shaderProgram;
    shaderProgram.loadVertexShader("src/shaders/basic.glvs");
    shaderProgram.loadFragmentShader("src/shaders/basic.glfs");
    shaderProgram.linkProgram();

    vertexPos3DLocation = glGetAttribLocation(shaderProgram.getProgramId(), "LVertexPos3D");
    if (vertexPos3DLocation == -1) {
        SDL_Log("Error getting vertexPos2DLocation");
    }

    GLint vertexNormal = glGetAttribLocation(shaderProgram.getProgramId(), "normals");;
    GLuint MVP = glGetUniformLocation(shaderProgram.getProgramId(), "MVP");
    GLuint lightID = glGetUniformLocation(shaderProgram.getProgramId(), "light");
    Mesh test;
    test.rawFileToMesh("res/meshes/monkeyhd.raw");

    const std::vector<Vertex>& meshVertices = test.getVertices();
    std::vector<glm::vec4> meshCopy;
    for each (const Vertex& v in meshVertices) {
        //SDL_Log("%f", v.getVector().x);
        meshCopy.push_back(v.getVector());
    }

    GLfloat* vertexData = new GLfloat[meshCopy.size() * 3];
    for (int i = 0; i < meshCopy.size(); i++) {
        vertexData[i * 3] = meshCopy.at(i).x;
        vertexData[i * 3 + 1] = meshCopy.at(i).y;
        vertexData[i * 3 + 2] = meshCopy.at(i).z;
    }

    const std::vector<Face>& meshFaces = test.getFaces();
    GLuint* indexData = new GLuint[meshFaces.size() * 3];
    for (int i = 0; i < meshFaces.size(); i++) {
        indexData[i * 3] = meshFaces[i].getVertexIndexAt(0);
        indexData[i * 3 + 1] = meshFaces[i].getVertexIndexAt(1);
        indexData[i * 3 + 2] = meshFaces[i].getVertexIndexAt(2);
    }

    std::vector<glm::vec3> normals(meshCopy.size());
    for (int i = 0; i < meshCopy.size(); i++) {
        const std::vector<int>& tmp = test.getVertices()[i].getFaces();
        glm::vec3 sum(0.f, 0.f, 0.f);
        for (int j = 0; j < tmp.size(); j++) {
            sum += test.getFaces()[tmp[j]].getNormal();
        }
        sum /= tmp.size();
        normals[i] = sum;
        //if (i % 2 == 0) normals[i] *= -1;
        //SDL_Log("%f, %f, %f", normals[i].x, normals[i].y, normals[i].z);
    }

    float theta = 0.01f;
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

        glm::mat4x4 xRotMat;
        theta += 0.01f;
        xRotMat[0][0] = 1.f; xRotMat[1][0] = 0.f;             xRotMat[2][0] = 0.f;            xRotMat[3][0] = 0.f;
        xRotMat[0][1] = 0.f; xRotMat[1][1] = cos(theta / 4);  xRotMat[2][1] = sin(theta / 4); xRotMat[3][1] = 0.f;
        xRotMat[0][2] = 0.f; xRotMat[1][2] = -sin(theta / 4); xRotMat[2][2] = cos(theta / 4); xRotMat[3][2] = 0.f;
        xRotMat[0][3] = 0.f; xRotMat[1][3] = 0.f;             xRotMat[2][3] = 0.f;            xRotMat[3][3] = 1.f;/**/

        glm::mat4x4 yRotMat;

        yRotMat[0][0] = cos(theta);  yRotMat[1][0] = 0.f; yRotMat[2][0] = sin(theta); yRotMat[3][0] = 0.f;
        yRotMat[0][1] = 0;           yRotMat[1][1] = 1.f; yRotMat[2][1] = 0;          yRotMat[3][1] = 0.f;
        yRotMat[0][2] = -sin(theta); yRotMat[1][2] = 0.f; yRotMat[2][2] = cos(theta); yRotMat[3][2] = 0.f;
        yRotMat[0][3] = 0.f;         yRotMat[1][3] = 0.f; yRotMat[2][3] = 0.f;        yRotMat[3][3] = 1.f;/**/

        glm::mat4x4 zRotMat;

        zRotMat[0][0] = cos(theta/3);  zRotMat[1][0] = sin(theta/3); zRotMat[2][0] = 0.f; zRotMat[3][0] = 0.f;
        zRotMat[0][1] = -sin(theta/3); zRotMat[1][1] = cos(theta/3); zRotMat[2][1] = 0.f; zRotMat[3][1] = 0.f;
        zRotMat[0][2] = 0.f;           zRotMat[1][2] = 0.f;          zRotMat[2][2] = 1.f; zRotMat[3][2] = 0.f;
        zRotMat[0][3] = 0.f;           zRotMat[1][3] = 0.f;          zRotMat[2][3] = 0.f; zRotMat[3][3] = 1.f;/**/

        glm::mat4x4 scaleMat;
        float s = 0.5;
        scaleMat[0][0] = s;   scaleMat[1][0] = 0.f; scaleMat[2][0] = 0.f; scaleMat[3][0] = 0.f;
        scaleMat[0][1] = 0.f; scaleMat[1][1] = s;   scaleMat[2][1] = 0.f; scaleMat[3][1] = 0.f;
        scaleMat[0][2] = 0.f; scaleMat[1][2] = 0.f; scaleMat[2][2] = s;   scaleMat[3][2] = 0.f;
        scaleMat[0][3] = 0.f; scaleMat[1][3] = 0.f; scaleMat[2][3] = 0.f; scaleMat[3][3] = 1.f;

        glm::mat4x4 modelViewMat = xRotMat * yRotMat * zRotMat * scaleMat;

        glm::vec3 testLight(2.f, 0.f, 0.f);

        //Logic
        

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, meshCopy.size() * 3 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshFaces.size() * 3 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
    
        glGenBuffers(1, &VNBO);
        glBindBuffer(GL_ARRAY_BUFFER, VNBO);
        glBufferData(GL_ARRAY_BUFFER, meshCopy.size() * 3 * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);

        //Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram.getProgramId());
        glUniformMatrix4fv(MVP, 1, GL_FALSE, &modelViewMat[0][0]);
        glUniform3fv(lightID, 1, &testLight[0]);

        glEnableVertexAttribArray(vertexPos3DLocation);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

        glEnableVertexAttribArray(vertexNormal);
        glBindBuffer(GL_ARRAY_BUFFER, VNBO);
        glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);

        glDrawElements(GL_TRIANGLES, 3 * meshFaces.size(), GL_UNSIGNED_INT, NULL);
        glDisableVertexAttribArray(vertexPos3DLocation);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &IBO);
        glDeleteBuffers(1, &VNBO);
        glUseProgram(NULL);

        SDL_GL_SwapWindow(window);
        //Manage fps
        int frameDelta = SDL_GetTicks() - frameStartTime;
        if (frameDelta < 1000 / FPS_CAP) {
            SDL_Delay(1000 / FPS_CAP - frameDelta);
        }
    }

    delete[] vertexData;
    delete[] indexData;

	return 0;
}