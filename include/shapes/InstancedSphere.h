#ifndef INSTANCED_SPHERE_H
#define INSTANCED_SPHERE_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <shader/shaderClass.h>
#include "buffers/VAO.h"
#include "buffers/VBO.h"
#include "buffers/EBO.h"

#include <vector>

class InstancedSphere
{
public:
    VAO shapeVAO;
    VBO shapeVBO;
    VBO instanceVBO;
    EBO shapeEBO;
    VBO colorVBO;
    VBO textureVBO;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::mat4> instanceMatrix;
    int hoveredSphereIndex = -1;
    std::vector<float> instanceColors;
    std::vector<int> instanceTextureID;
    int textureSize = -1;

    float radius;
    glm::vec3 position;
    glm::mat4 modelMatrix;
    bool initialized = false; // Track if sphere is initialized

    InstancedSphere(float radius, int sectorCount, int stackCount, std::vector<glm::mat4> mat4s, std::vector<int> instanceTextures, int textureSize);
    void drawInstanced(Shader &shader, size_t instanceCount);
    void updateInstanceData(const std::vector<glm::vec3> &positions);
    void generateMesh(float radius, int sectorCount, int stackCount);
    void setHoveredSphere(int index, bool didClick);
    ~InstancedSphere();
};

#endif
