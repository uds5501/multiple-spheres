#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in mat4 instanceMatrix;
layout (location = 6) in vec4 instanceColor;

out vec3 Normal;
out vec3 currentPosition;
out vec4 color;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
    currentPosition = vec3(instanceMatrix * vec4(aPos, 1.0f));
    gl_Position = camMatrix * vec4(currentPosition, 1.0);
    Normal = normalize(aNormal);
    color = instanceColor;
}