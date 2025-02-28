#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H
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

class LightSource
{
public:
    VAO lightVAO;
    VBO lightVBO;
    EBO lightEBO;

    std::vector<float> lightVertices;
    std::vector<unsigned int> lightIndices;
    glm::vec3 lightPos;
    glm::vec4 lightColor;
    glm::mat4 lightModel;
    bool initialized = false;

    LightSource(glm::vec3 &lightPos, glm::vec4 &lightColor);
    void draw(Shader &shader);
    void updateShaders(Shader &lightShader, Shader &instanceShader);
    ~LightSource();
};

#endif
