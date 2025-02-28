#include "shapes/LightSource.h"
#include <glm/glm.hpp>
#include <cmath>

LightSource::LightSource(glm::vec3 &lightPos, glm::vec4 &lightColor)
{
    initialized = true;
    this->lightPos = lightPos;
    this->lightColor = lightColor;
    this->lightModel = glm::translate(glm::mat4(1.0f), lightPos);
    this->lightVertices = {
        //  COORDINATES
        -0.1f, -0.1f, 0.1f,
        -0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, 0.1f,
        -0.1f, 0.1f, 0.1f,
        -0.1f, 0.1f, -0.1f,
        0.1f, 0.1f, -0.1f,
        0.1f, 0.1f, 0.1f};

    this->lightIndices = {
        0, 1, 2, 0, 2, 3,
        0, 4, 7, 0, 7, 3,
        3, 7, 6, 3, 6, 2,
        2, 6, 5, 2, 5, 1,
        1, 5, 4, 1, 4, 0,
        4, 5, 6, 4, 6, 7};
    lightVAO.Bind();
    lightVBO = VBO(lightVertices.data(), static_cast<GLsizeiptr>(sizeof(float) * lightVertices.size()));
    lightEBO = EBO(lightIndices.data(), static_cast<GLsizeiptr>(sizeof(unsigned int) * lightIndices.size()));

    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *)0);
    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();
}

void LightSource::updateShaders(Shader &lightShader, Shader &instanceShader)
{
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

    glUniform4f(glGetUniformLocation(instanceShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(instanceShader.ID, "lightPosition"), lightPos.x, lightPos.y, lightPos.z);
}

void LightSource::draw(Shader &shader)
{
    lightVAO.Bind();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glDrawElements(GL_TRIANGLES, lightIndices.size, GL_UNSIGNED_INT, 0);
    lightVAO.Unbind();
}

LightSource::~LightSource()
{
    std::cout << "DELETING LIGHT SOURCE\n";
    if (initialized)
    {
        lightVAO.Delete();
        lightVBO.Delete();
        lightEBO.Delete();
    }
}
