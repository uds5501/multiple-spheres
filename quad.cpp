#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/shaderClass.h"
#include "buffers/VAO.h"
#include "buffers/VBO.h"
#include "buffers/EBO.h"
#include "camera/Camera.h"
#include "shapes/Sphere.h"
#include "shapes/InstancedSphere.h"
#include "stb_image.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Quad vertices: (X, Y, U, V)
float quadVertices[] = {
	// Positions   // Texture Coords
	-1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
	1.0f, -1.0f, 1.0f, 0.0f,  // Bottom-right
	1.0f, 1.0f, 1.0f, 1.0f,	  // Top-right
	-1.0f, 1.0f, 0.0f, 1.0f	  // Top-left
};

// Quad indices (for an EBO)
unsigned int indices[] = {
	0, 1, 2, // First triangle
	2, 3, 0	 // Second triangle
};

GLuint loadTexture(const char *filename)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set texture wrapping options (repeat mode)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load the image using stb_image
	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(true); // Flip image to match OpenGL's coordinate system
	unsigned char *data = stbi_load(filename, &width, &height, &numChannels, 0);

	if (data)
	{
		GLenum format = (numChannels == 3) ? GL_RGB : GL_RGBA; // Handle RGB and RGBA images
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps for better scaling

		std::cout << "Loaded texture: " << filename << " (" << width << "x" << height << ")\n";
	}
	else
	{
		std::cerr << "Failed to load texture: " << filename << std::endl;
	}

	stbi_image_free(data); // Free image memory
	return textureID;
}

int main()
{
	// Initialize GLFW
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(width, height, "Quads", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();
	if (!gladLoadGL())
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}
	glViewport(0, 0, width, height);
	stbi_set_flip_vertically_on_load(true);

	GLuint tex1 = loadTexture("textures/Tiles086_2K-JPG_Color.jpg");
	// GLuint tex2 = loadTexture("textures/Tiles100_2K-JPG_Color.jpg");
	// GLuint tex3 = loadTexture("textures/Wood009_2K-JPG_Color.jpg");
	// GLuint tex4 = loadTexture("textures/Wood066_2K-JPG_Color.jpg");
	// std::vector<GLuint> textureIDs = {tex1, tex2, tex3, tex4};

	Shader quadShader("shaders/quad.vert", "shaders/quad.frag");

	VAO quadVAO;
	quadVAO.Bind();
	quadVAO.PrintID();

	VBO VBO2(quadVertices, sizeof(quadVertices));
	EBO EBO2(indices, sizeof(indices));

	quadVAO.LinkAttrib(VBO2, 0, 2, GL_FLOAT, 4 * sizeof(float), (void *)0);
	quadVAO.LinkAttrib(VBO2, 1, 2, GL_FLOAT, 4 * sizeof(float), (void *)(2 * sizeof(float)));

	quadVAO.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();

	// for (size_t i = 0; i < textureIDs.size(); i++)
	// {
	// 	glActiveTexture(GL_TEXTURE0 + i);
	// 	glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
	// 	std::string uniformName = "textures[" + std::to_string(i) + "]";
	// 	glUniform1i(glGetUniformLocation(instanceShader.ID, uniformName.c_str()), i);
	// }
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	std::string uniformName = "texture1";
	glUniform1i(glGetUniformLocation(quadShader.ID, uniformName.c_str()), 0);

	glEnable(GL_DEPTH_TEST);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the background color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		quadShader.Activate();

		quadVAO.Bind();
		glUniform1i(glGetUniformLocation(quadShader.ID, uniformName.c_str()), 0);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	quadVAO.Delete();
	VBO2.Delete();
	EBO2.Delete();

	quadShader.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
