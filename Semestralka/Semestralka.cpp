//#include <iostream>
//#include <string>
//#include <vector>
//#include <map>
//
//#include "pgr.h"
//#include "Shader.h"
//
//const char* mainVertexSource = R"(
//#version 330
//
//layout(location=0) in vec3 a_Position;
//layout(location=1) in vec3 a_Normal;
//layout(location=2) in vec2 a_TexCoords;
//layout(location=3) in ivec4 a_BoneIDs;
//layout(location=4) in vec4 a_BoneWeights;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//
//out vec2 TexCoords;
//out vec3 WorldNormal;
//flat out ivec4 BoneIDs;
//out vec4 BoneWeights;
//
//void main() {
//    TexCoords = a_TexCoords;
//    WorldNormal = mat3(model) * a_Normal;
//    BoneIDs = a_BoneIDs;
//    BoneWeights = a_BoneWeights;
//
//    gl_Position = projection * view * model * vec4(a_Position, 1.0);
//}
//)";
//
//const char* mainFragmentSource = R"(
//#version 330
//
//in vec2 TexCoords;
//in vec3 WorldNormal;
//flat in ivec4 BoneIDs;
//in vec4 BoneWeights;
//
//uniform sampler2D texture_diffuse1;
//uniform int activeBoneIndex;
//
//out vec4 color;
//
//void main() {
//    vec4 highlightColor = vec4(0.0, 0.0, 1.0, 1.0);
//    for(int i = 0; i < 4; ++i) {
//        if(BoneIDs[i] == activeBoneIndex) {
//            if(BoneWeights[i] > 0.7) {
//                highlightColor = BoneWeights[i] * vec4(0.0, 1.0, 0.0, 1.0);
//            } else if(BoneWeights[i] > 0.4) {
//                highlightColor = BoneWeights[i] * vec4(0.0, 1.0, 1.0, 1.0);
//            }
//        }
//    }
//    float r = BoneIDs[0] == 0 ? 1.0 : 800000000.0 / BoneIDs[0];
//    color = vec4(r, 0.0, 0.0, 1.0);
//}
//)";
//
//class Shader {
//    GLuint id;
//
//public:
//    Shader(const std::string& path);
//    Shader(const char* vertexSource, const char* fragmentSource);
//    ~Shader();
//    void use();
//    void setBool(const char* name, bool value);
//    void setInt(const char* name, int value);
//    void setFloat(const char* name, float value);
//    void setMat4(const char* name, const glm::mat4& value);
//};
//
//class Camera {
//public:
//    glm::vec3 Position;
//    glm::vec3 Front;
//    glm::vec3 Up, WorldUp;
//    glm::vec3 Right;
//    float Yaw;
//    float Pitch;
//
//    Camera() {
//        Yaw = 0.0f;
//        Pitch = 0.0f;
//        Position = glm::vec3(0.0f, 0.0f, 0.0f);
//        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
//        calculateVectors();
//    }
//    glm::mat4 GetProjectionMatrix() {
//        return glm::perspective(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
//    }
//    glm::mat4 GetViewMatrix() {
//        return glm::lookAt(Position, Position + Front, Up);
//    }
//
//    void calculateVectors() {
//        glm::vec3 front;
//        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//        front.y = sin(glm::radians(Pitch));
//        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//        Front = glm::normalize(front);
//
//        Right = glm::normalize(glm::cross(Front, WorldUp));
//        Up = glm::normalize(glm::cross(Right, Front));
//    }
//};
//
//struct Vertex {
//    glm::vec3 Position;
//    glm::vec3 Normal;
//    glm::vec2 TexCoords;
//    unsigned int BoneIDs[4] = { 0, 0, 0, 0 };
//    float BoneWeights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
//
//    void AddBoneData(unsigned int boneID, float weight) {
//        for (unsigned int i = 0; i < 4; ++i) {
//            if (BoneWeights[i] == 0.0f) {
//                BoneIDs[i] = boneID;
//                BoneWeights[i] = weight;
//                break;
//            }
//        }
//    }
//};
//
//struct Texture {
//    GLuint id;
//    std::string path;
//    std::string type;
//};
//
//class Mesh {
//    GLuint VAO, VBO, EBO;
//    std::vector<Vertex> vertices;
//    std::vector<unsigned int> indices;
//    std::vector<Texture> textures;
//public:
//    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
//    void Draw(Shader& shader);
//private:
//    void init();
//};
//
//class Model {
//    std::vector<Mesh> meshes;
//    std::map<std::string, unsigned int> boneMap;
//    std::string directory;
//public:
//    glm::vec3 Scale;
//    float Pitch;
//    float Yaw;
//    Model(const std::string& path);
//    void Draw(Shader& shader);
//private:
//    void loadModel(const std::string& path);
//    void processNode(aiNode* node, const aiScene* scene);
//    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
//    void processBone(aiBone* bone, const aiScene* scene, std::vector<Vertex>& vertices);
//    unsigned int getBoneID(aiBone* bone);
//    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);
//};
//
//std::vector<Vertex> rectangleVertices = {
//    { glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
//    { glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
//    { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
//    { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) }
//};
//
//std::vector<unsigned int> rectangleIndces = { 0, 1, 2, 0, 2, 3 };
//
//Shader* mainShader;
//Camera camera;
//Mesh* rectangle;
//Model* testModel;
//int counter = 0;
//int activeBoneIndex = 0;
//
//std::vector<Texture> loadedTextures;
//
//void init() {
//    mainShader = new Shader(mainVertexSource, mainFragmentSource);
//    testModel = new Model("data/skeleton/skeleton.FBX");
//    rectangle = new Mesh(rectangleVertices, rectangleIndces, {});
//
//    camera.Position.y = 4.0f;
//    camera.Position.z = 10.0f;
//    camera.Yaw = -90.0f;
//
//    testModel->Pitch = -90.0f;
//    testModel->Scale = glm::vec3(0.1f);
//
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//}
//
//void draw() {
//
//    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    mainShader->use();
//
//    mainShader->setInt("activeBoneIndex", activeBoneIndex);
//
//    mainShader->setMat4("projection", camera.GetProjectionMatrix());
//    mainShader->setMat4("view", camera.GetViewMatrix());
//        
//    testModel->Draw(*mainShader);
//}
//
//void closeCb() {
//    delete mainShader;
//    delete testModel;
//    delete rectangle;
//}
//
//void displayCb() {
//
//    draw();
//
//    glutSwapBuffers();
//}
//
//void timerCb(int) {
//
//    testModel->Yaw += 2.0f;
//
//    camera.calculateVectors();
//
//    glutTimerFunc(33, timerCb, 0);
//
//    glutPostRedisplay();
//}
//
//int main(int argc, char* argv[])
//{
//
//    glutInit(&argc, argv);
//
//    glutInitContextVersion(pgr::OGL_VER_MAJOR, 3);
//    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
//
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//    glutInitWindowSize(1920, 1080);
//
//    glutCreateWindow("Gothic");
//
//    glutDisplayFunc(displayCb);
//    glutCloseFunc(closeCb);
//    glutTimerFunc(33, timerCb, 0);
//
//    if (!pgr::initialize(pgr::OGL_VER_MAJOR, 3)) {
//        pgr::dieWithError("pgr init failed, required OpenGL not supported.");
//    }
//
//    init();
//
//    glutMainLoop();
//
//    return 0;
//}
//
//Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
//    : vertices(vertices), indices(indices), textures(textures)
//{
//    init();
//}
//
//void Mesh::Draw(Shader& shader)
//{
//    unsigned int diffuseNum = 1;
//    unsigned int specularNum = 1;
//    for (unsigned int i = 0; i < textures.size(); ++i) {
//        glActiveTexture(GL_TEXTURE0 + i);
//
//        std::string name = textures[i].type;
//        std::string num;
//        if (name == "texture_diffuse") {
//            num = std::to_string(diffuseNum++);
//        }
//        else if (name == "texture_specular") {
//            num = std::to_string(specularNum++);
//        }
//        shader.setInt((name + num).c_str(), i);
//        glBindTexture(GL_TEXTURE_2D, textures[i].id);
//    }
//    glActiveTexture(GL_TEXTURE0);
//
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//}
//
//void Mesh::init()
//{
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//    glEnableVertexAttribArray(1);
//
//    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
//    glEnableVertexAttribArray(2);
//
//    glVertexAttribPointer(3, 4, GL_INT, false, sizeof(Vertex), (void*)(8 * sizeof(float)));
//    glEnableVertexAttribArray(3);
//
//    glVertexAttribPointer(4, 4, GL_FLOAT, false, sizeof(Vertex), (void*)(8 * sizeof(float) + 4 * sizeof(unsigned int)));
//    glEnableVertexAttribArray(4);
//
//    glGenBuffers(1, &EBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//
//    glBindVertexArray(0);
//}
//
//Shader::Shader(const std::string& path)
//{
//    GLuint shaders[] = {
//        pgr::createShaderFromFile(GL_VERTEX_SHADER, path + ".vert"),
//        pgr::createShaderFromFile(GL_FRAGMENT_SHADER, path + ".frag"),
//        0
//    };
//
//    id = pgr::createProgram(shaders);
//}
//
//Shader::Shader(const char* vertexSource, const char* fragmentSource)
//{
//    GLuint shaders[] = {
//        pgr::createShaderFromSource(GL_VERTEX_SHADER, vertexSource),
//        pgr::createShaderFromSource(GL_FRAGMENT_SHADER, fragmentSource),
//        0
//    };
//
//    id = pgr::createProgram(shaders);
//}
//
//Shader::~Shader()
//{
//    pgr::deleteProgramAndShaders(id);
//}
//
//void Shader::use()
//{
//    glUseProgram(id);
//}
//
//void Shader::setBool(const char* name, bool value)
//{
//    glUniform1i(glGetUniformLocation(id, name), (int)value);
//}
//
//void Shader::setInt(const char* name, int value)
//{
//    glUniform1i(glGetUniformLocation(id, name), value);
//}
//
//void Shader::setFloat(const char* name, float value) {
//    glUniform1f(glGetUniformLocation(id, name), value);
//}
//
//void Shader::setMat4(const char* name, const glm::mat4& value)
//{
//    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, &value[0][0]);
//}
//
//Model::Model(const std::string& path)
//{
//    Scale = glm::vec3(1.0f, 1.0f, 1.0f);
//    Yaw = 0.0f;
//    Pitch = 0.0f;
//    loadModel(path);
//}
//
//void Model::Draw(Shader& shader)
//{
//    glm::mat4 modelMatrix = glm::mat4(1.0);
//    
//    modelMatrix = glm::rotate(modelMatrix, glm::radians(Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
//    modelMatrix = glm::rotate(modelMatrix, glm::radians(Yaw), glm::vec3(0.0f, cos(glm::radians(Pitch)), sin(glm::radians((Pitch)))));
//    modelMatrix = glm::scale(modelMatrix, Scale);
//    mainShader->setMat4("model", modelMatrix);
//
//    for (auto& mesh : meshes) {
//        mesh.Draw(shader);
//    }
//}
//
//void Model::loadModel(const std::string& path)
//{
//    Assimp::Importer importer;
//    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//
//    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
//        return;
//    }
//
//    directory = path.substr(0, path.find_last_of('/'));
//
//    processNode(scene->mRootNode, scene);
//
//}
//
//void Model::processNode(aiNode* node, const aiScene* scene)
//{
//    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
//        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//        meshes.push_back(processMesh(mesh, scene));
//    }
//
//    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
//        processNode(node->mChildren[i], scene);
//    }
//}
//
//Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
//{
//    std::vector<Vertex> vertices;
//    std::vector<unsigned int> indices;
//    std::vector<Texture> textures;
//
//    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
//        Vertex vertex;
//
//        glm::vec3 position;
//        position.x = mesh->mVertices[i].x;
//        position.y = mesh->mVertices[i].y;
//        position.z = mesh->mVertices[i].z;
//        vertex.Position = position;
//
//        glm::vec3 normal;
//        normal.x = mesh->mNormals[i].x;
//        normal.y = mesh->mNormals[i].y;
//        normal.z = mesh->mNormals[i].z;
//        vertex.Normal = normal;
//
//        if (mesh->mTextureCoords[0]) {
//            glm::vec2 uv;
//            uv.x = mesh->mTextureCoords[0][i].x;
//            uv.y = mesh->mTextureCoords[0][i].y;
//            vertex.TexCoords = uv;
//        }
//        else {
//            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
//        }
//
//        vertices.push_back(vertex);
//    }
//
//    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
//        const aiFace& face = mesh->mFaces[i];
//        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
//            indices.push_back(face.mIndices[j]);
//        }
//    }
//
//    if (mesh->mMaterialIndex >= 0) {
//        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//
//        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//    }
//
//    for (unsigned int i = 0; i < mesh->mNumBones; ++i) {
//        aiBone* bone = mesh->mBones[i];
//        processBone(bone, scene, vertices);
//    }
//
//    for (unsigned int i = 0; i < 100; ++i) {
//        std::cout << "Vertex " << i << " - Bone ID: " << vertices[i].BoneIDs[0] << "\n";
//    }
//    
//    return Mesh(vertices, indices, textures);
//}
//
//void Model::processBone(aiBone* bone, const aiScene* scene, std::vector<Vertex>& vertices)
//{
//    unsigned int boneID = getBoneID(bone);
//    std::string boneName(bone->mName.C_Str());
//    std::cout << boneName << " ID " << boneMap[boneName] << " has " << bone->mNumWeights << " weights\n";
//
//    for (unsigned int i = 0; i < bone->mNumWeights; ++i) {
//        const aiVertexWeight& vw = bone->mWeights[i];
//
//        if (vw.mVertexId >= vertices.size()) {
//            std::cout << "ERROR";
//        }
//
//        vertices[vw.mVertexId].AddBoneData(boneID, vw.mWeight);
//    }
//}
//
//unsigned int Model::getBoneID(aiBone* bone)
//{
//    std::string boneName(bone->mName.C_Str());
//    if (boneMap.find(boneName) == boneMap.end()) {
//        boneMap[boneName] = (int)boneMap.size();
//    }
//    return boneMap[boneName];
//}
//
//std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName)
//{
//    std::vector<Texture> textures;
//
//    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
//        aiString path;
//        material->GetTexture(type, i, &path);
//
//        std::string texturePath = directory + "/" + std::string(path.C_Str());
//
//        bool exists = false;
//
//        for (Texture& texture : loadedTextures) {
//            if (texture.path == texturePath) {
//                textures.push_back(texture);
//                exists = true;
//                break;
//            }
//        }
//
//        if (!exists) {
//            Texture texture;
//            texture.id = pgr::createTexture(texturePath);
//            texture.type = typeName;
//            texture.path = texturePath;
//            textures.push_back(texture);
//            loadedTextures.push_back(texture);
//        }
//    }
//
//    return textures;
//}
