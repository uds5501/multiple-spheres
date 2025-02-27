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

VBO::VBO(std::vector<float> colors)
{

	if (!init)
	{
		init = true;
		glGenBuffers(1, &ID);
	}
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_DYNAMIC_DRAW);
}
VBO::VBO(std::vector<int> &data)
{
	if (!init)
	{
		init = true;
		glGenBuffers(1, &ID);
	}
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(int), data.data(), GL_STATIC_DRAW);
}

void VBO::UpdateData(std::vector<float> &colors)
{
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(float), colors.data());
	Unbind();
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