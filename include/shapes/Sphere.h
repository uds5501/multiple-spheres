#ifndef SPHERE_H
#define SPHERE_H
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

class Sphere {
public:
    VAO shapeVAO; 
    VBO shapeVBO;
    EBO shapeEBO;
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    float radius;
    glm::vec3 position;
    glm::mat4 modelMatrix;
    bool initialized = false;  // Track if sphere is initialized

    Sphere(float radius, int sectorCount, int stackCount, glm::vec3 position); // Parameterized constructor
    void draw(Shader &shaderProgram);
    void generateMesh(float radius, int sectorCount, int stackCount, glm::vec3 position);
    void updateModelMatrix();
    ~Sphere();
};

#endif
