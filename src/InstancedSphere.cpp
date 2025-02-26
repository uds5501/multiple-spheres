#include "shapes/InstancedSphere.h"
#include <glm/glm.hpp>
#include <cmath>

// Sphere::Sphere() : VAO(0), VBO(0), EBO(0), initialized(false) {}

InstancedSphere::InstancedSphere(float radius, int sectorCount, int stackCount, std::vector<glm::mat4> mat4s)
{
    initialized = true;
    instanceMatrix = mat4s;

    generateMesh(radius, sectorCount, stackCount);

    // Debug prints
    std::cout << "Vertices generated: " << vertices.size() / 3 << " vertices\n";
    std::cout << "Indices generated: " << indices.size() << " indices\n";

    shapeVAO.Bind();
    instanceColors.resize(instanceMatrix.size() * 4, 1.0f);

    // Check for OpenGL errors

    shapeVBO = VBO(vertices.data(), static_cast<GLsizeiptr>(sizeof(float) * vertices.size()));
    shapeEBO = EBO(indices.data(), static_cast<GLsizeiptr>(sizeof(unsigned int) * indices.size()));

    shapeVAO.LinkAttrib(shapeVBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *)0); // saved the basic mesh points.
    shapeVAO.LinkAttrib(shapeVBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float))); // save the indices from the same shape buffer

    instanceVBO = VBO(mat4s); 
    for (int i = 0; i < 4; i++)
    {
        shapeVAO.LinkAttrib(instanceVBO, 2 + i, 4, GL_FLOAT, sizeof(glm::mat4), (void *)(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(2 + i, 1); // This tells OpenGL this is per-instance
    }
    colorVBO = VBO(instanceColors);
    shapeVAO.LinkAttrib(colorVBO, 6, 4, GL_FLOAT, 4 * sizeof(float), (void *)0); // link the indices from the color buffer.
    glVertexAttribDivisor(6, 1);

    shapeVAO.Unbind();

    shapeVBO.Unbind();
    instanceVBO.Unbind();
    colorVBO.Unbind();
    shapeEBO.Unbind();
}

void InstancedSphere::generateMesh(float radius, int sectorCount, int stackCount)
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

void InstancedSphere::updateInstanceData(const std::vector<glm::vec3> &positions)
{
    // Update transformation matrices for all instances
    for (size_t i = 0; i < positions.size(); i++)
    {
        instanceMatrix[i] = glm::translate(glm::mat4(1.0f), positions[i]);
    }

    // Update instance buffer
    instanceVBO.Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    sizeof(glm::mat4) * instanceMatrix.size(),
                    &instanceMatrix[0]);
}

void InstancedSphere::drawInstanced(Shader &shader, size_t instanceCount)
{
    if (!initialized)
        return;

    shapeVAO.Bind();
    glDrawElementsInstanced(GL_TRIANGLES,
                            static_cast<GLsizei>(indices.size()),
                            GL_UNSIGNED_INT,
                            0,
                            static_cast<GLsizei>(instanceCount));
    shapeVAO.Unbind();
}

void InstancedSphere::setHoveredSphere(int index)
{
    if (hoveredSphereIndex == index)
        return;

    // Reset previous hover
    if (hoveredSphereIndex >= 0 && hoveredSphereIndex < instanceColors.size() / 4)
    {
        int offset = hoveredSphereIndex * 4;
        instanceColors[offset] = 1.0f;     // R
        instanceColors[offset + 1] = 1.0f; // G
        instanceColors[offset + 2] = 1.0f; // B
        instanceColors[offset + 3] = 1.0f; // A
    }

    // Set new hover
    hoveredSphereIndex = index;
    if (index >= 0 && index < instanceColors.size() / 4)
    {
        int offset = index * 4;
        instanceColors[offset] = 1.0f;     // R - yellow-ish aura
        instanceColors[offset + 1] = 1.0f; // G
        instanceColors[offset + 2] = 0.0f; // B
        instanceColors[offset + 3] = 1.0f; // A

        colorVBO.UpdateData(instanceColors);
    }
}

// Destructor
InstancedSphere::~InstancedSphere()
{
    std::cout << "DELETING InstancedSphere\n";
    shapeVAO.PrintID();
    if (initialized)
    {
        shapeVAO.Delete();
        shapeVBO.Delete();
        instanceVBO.Delete();
        shapeEBO.Delete();
    }
}
