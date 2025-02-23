#include "shapes/Sphere.h"
#include <glm/glm.hpp>
#include <cmath>

// Sphere::Sphere() : VAO(0), VBO(0), EBO(0), initialized(false) {}

Sphere::Sphere(float radius, int sectorCount, int stackCount, glm::vec3 position) : position(position)
{
    initialized = true;

    generateMesh(radius, sectorCount, stackCount, position);
    updateModelMatrix();

    // Debug prints
    std::cout << "Vertices generated: " << vertices.size() / 3 << " vertices\n";
    std::cout << "Indices generated: " << indices.size() << " indices\n";

    shapeVAO.Bind();
    // Check for OpenGL errors
    GLenum err;
    
    shapeVBO = VBO(vertices.data(), static_cast<GLsizeiptr>(sizeof(float) * vertices.size()));
    shapeEBO = EBO(indices.data(), static_cast<GLsizeiptr>(sizeof(unsigned int) * indices.size()));

    shapeVAO.LinkAttrib(shapeVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *)0);
    shapeVAO.LinkAttrib(shapeVBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    shapeVAO.Unbind();
    shapeVBO.Unbind();
    shapeEBO.Unbind();
}

void Sphere::generateMesh(float radius, int sectorCount, int stackCount, glm::vec3 position)
{
    for (int i = 0; i <= stackCount; ++i)
    {
        float stackAngle = M_PI / 2 - i * (M_PI / stackCount);
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j)
        {
            float sectorAngle = j * (2 * M_PI / sectorCount);
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
            // Normal vector (normalized position relative to sphere center)
            vertices.push_back(normal.x);   
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

        }
    }

    for (int i = 0; i < stackCount; ++i)
    {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;
        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void Sphere::updateModelMatrix()
{
    modelMatrix = glm::translate(glm::mat4(1.0f), position);
}

void Sphere::draw(Shader &shaderProgram)
{
    if (!initialized) {
        std::cout << "Sphere not initialized!\n";
        return;
    }
    updateModelMatrix();
    GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");

    shapeVAO.Bind();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

    // Check for errors after draw
    // while ((err = glGetError()) != GL_NO_ERROR)
    // {
    //     std::cout << "OpenGL error after draw: " << err << "\n";
    // }
}

// Destructor
Sphere::~Sphere()
{
    std::cout << "DELETING SPHERE\n";
    shapeVAO.PrintID();
    if (initialized)
    {
        shapeVAO.Delete();
        shapeVBO.Delete();
        shapeEBO.Delete();
    }
}
