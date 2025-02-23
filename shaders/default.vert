#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 currentPosition;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
    currentPosition = vec3(model * vec4(aPos, 1.0f));
    gl_Position = camMatrix * vec4(currentPosition, 1.0);
    Normal = normalize(aNormal);
}