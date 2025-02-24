#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector>

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	bool init = false;
	VBO();
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(GLfloat *vertices, GLsizeiptr size);
	VBO(std::vector<glm::mat4> &mat4s);

	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif