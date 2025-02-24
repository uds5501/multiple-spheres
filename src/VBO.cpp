#include "buffers/VBO.h"

VBO::VBO()
{
	init = true;
	glGenBuffers(1, &ID);
}

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(GLfloat *vertices, GLsizeiptr size)
{
	if (!init)
	{
		init = true;
		glGenBuffers(1, &ID);
	}
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VBO::VBO(std::vector<glm::mat4> &mat4s)
{
	if (!init)
	{
		init = true;
		glGenBuffers(1, &ID);
	}
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, mat4s.size() * sizeof(glm::mat4), mat4s.data(), GL_STATIC_DRAW);
}

// Binds the VBO
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}