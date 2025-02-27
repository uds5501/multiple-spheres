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

GLfloat lightVertices[] =
	{ //     COORDINATES     //
		-0.1f, -0.1f, 0.1f,
		-0.1f, -0.1f, -0.1f,
		0.1f, -0.1f, -0.1f,
		0.1f, -0.1f, 0.1f,
		-0.1f, 0.1f, 0.1f,
		-0.1f, 0.1f, -0.1f,
		0.1f, 0.1f, -0.1f,
		0.1f, 0.1f, 0.1f};

GLuint lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7};

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

int findHoveredSphere(const glm::vec3 &rayOrigin, const glm::vec3 &rayDir,
					  const std::vector<glm::mat4> &transforms, float radius)
{
	int closestSphereIndex = -1;
	float closestDist = std::numeric_limits<float>::max();

	for (size_t i = 0; i < transforms.size(); i++)
	{
		// Extract position from transform matrix
		glm::vec3 sphereCenter = glm::vec3(transforms[i][3]);

		// Extract scale from transform matrix (assuming uniform scaling)
		float scale = glm::length(glm::vec3(transforms[i][0]));
		float scaledRadius = radius * scale;

		// Ray-sphere intersection
		glm::vec3 oc = rayOrigin - sphereCenter;
		float a = glm::dot(rayDir, rayDir);
		float b = 2.0f * glm::dot(oc, rayDir);
		float c = glm::dot(oc, oc) - scaledRadius * scaledRadius;
		float discriminant = b * b - 4 * a * c;

		if (discriminant >= 0)
		{
			float dist = (-b - sqrt(discriminant)) / (2.0f * a);
			if (dist > 0 && dist < closestDist)
			{
				closestDist = dist;
				closestSphereIndex = i;
			}
		}
	}

	return closestSphereIndex;
}

int main(int argc, char *argv[])
{
	// Initialize GLFW
	glfwInit();
	float sphereRadius = 0.6f;
	int numberOfSpheres = 10000;

	// Check if CLI arguments are provided
	if (argc > 1)
		sphereRadius = std::stof(argv[1]); // First argument: Sphere radius
	if (argc > 2)
		numberOfSpheres = std::stoi(argv[2]); // Second argument: Number of spheres

	std::cout << "Sphere Radius: " << sphereRadius << std::endl;
	std::cout << "Number of Spheres: " << numberOfSpheres << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(width, height, "ALotOfSpheres", NULL, NULL);

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
	GLuint tex2 = loadTexture("textures/Tiles100_2K-JPG_Color.jpg");
	GLuint tex3 = loadTexture("textures/Wood009_2K-JPG_Color.jpg");
	GLuint tex4 = loadTexture("textures/Wood066_2K-JPG_Color.jpg");
	std::vector<GLuint> textureIDs = {tex1, tex2, tex3, tex4};

	Shader lightShader("shaders/light.vert", "shaders/light.frag");
	Shader instanceShader("shaders/instance.vert", "shaders/default.frag");

	std::vector<glm::mat4> transforms;
	std::vector<int> instanceTextures;
	float spread = 200.0f;
	transforms.reserve(numberOfSpheres);
	instanceTextures.reserve(numberOfSpheres);
	for (int i = 0; i < numberOfSpheres; i++)
	{
		// Generate random position in a large range
		float x = static_cast<float>(rand()) / RAND_MAX * spread - (spread / 2); // Range: -spread/2 to spread/2
		float y = static_cast<float>(rand()) / RAND_MAX * spread - (spread / 2);
		float z = static_cast<float>(rand()) / RAND_MAX * spread - (spread / 2);

		// Apply minor random rotation and scaling for variety
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
		transform = glm::scale(transform, glm::vec3(0.5f + static_cast<float>(rand()) / RAND_MAX * 1.0f)); // Scale 0.5 to 2.5

		transforms.push_back(transform);
		int randomTexIndex = rand() % textureIDs.size();
		instanceTextures.push_back(randomTexIndex);
	}
	InstancedSphere *spheres = new InstancedSphere(sphereRadius, 30, 30, transforms, instanceTextures);

	VAO lightVAO;
	lightVAO.Bind();
	lightVAO.PrintID();

	VBO VBO2(lightVertices, sizeof(lightVertices));
	EBO EBO2(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *)0);
	lightVAO.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();

	glm::vec3 lightPos = glm::vec3(0.0f, 0.5f, 0.0f);
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	instanceShader.Activate();
	glUniform4f(glGetUniformLocation(instanceShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(instanceShader.ID, "lightPosition"), lightPos.x, lightPos.y, lightPos.z);
	for (size_t i = 0; i < textureIDs.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
		std::string uniformName = "textures[" + std::to_string(i) + "]";
		glUniform1i(glGetUniformLocation(instanceShader.ID, uniformName.c_str()), i);
	}

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 5.0f));
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the background color
		glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0)
		{ // If last update was 1 second ago
			double fps = nbFrames / (currentTime - lastTime);
			nbFrames = 0;
			lastTime = currentTime;
			std::string newTitle = "ALotOfSpheres | FPS: " + std::to_string((int)fps);
			glfwSetWindowTitle(window, newTitle.c_str());
		}
		// Activate main object shader
		instanceShader.Activate();

		// Update camera matrix & position
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 1000.0f);
		camera.Matrix(instanceShader, "camMatrix");

		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		glm::vec3 rayOrigin = camera.Position;
		glm::vec3 rayDir = camera.getRayDirection(mouseX, mouseY, width, height);

		instanceShader.Activate();
		int idx = findHoveredSphere(rayOrigin, rayDir, transforms, 0.6);
		spheres->setHoveredSphere(idx);
		spheres->drawInstanced(instanceShader, numberOfSpheres);

		// Now activate light shader and render the light source
		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();

		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		lightVAO.Unbind();

		// Swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	lightVAO.Delete();
	VBO2.Delete();
	EBO2.Delete();

	lightShader.Delete();
	// shaderProgram.Delete();
	instanceShader.Delete();

	delete spheres;
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
