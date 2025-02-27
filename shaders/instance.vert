#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instanceMatrix;
layout (location = 7) in vec4 instanceColor;
layout (location = 8) in int textureIndex;

out vec3 Normal;
out vec3 currentPosition;
out vec4 color;
out vec2 textCoords;
flat out int TexIndex; 

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
    currentPosition = vec3(instanceMatrix * vec4(aPos, 1.0f));
    gl_Position = camMatrix * vec4(currentPosition, 1.0);
    Normal = normalize(aNormal);
    color = instanceColor;
    TexIndex = textureIndex;
    textCoords = aTexCoords;
}