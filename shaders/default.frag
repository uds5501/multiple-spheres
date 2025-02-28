#version 330 core

out vec4 FragColor;
in vec3 Normal;
in vec3 currentPosition;
in vec4 color;
in vec2 textCoords;
flat in int TexIndex;

uniform vec4 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform sampler2D textures[4];

void main()
{
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - currentPosition);
    float distance = length(lightPosition - currentPosition);

    // Adjusted Attenuation to avoid extreme fading
    float Kc = 1.0;
    float Kl = 0.01;  // Lower attenuation
    float Kq = 0.002; // Lower attenuation
    float attenuation = 1.0 / (Kc + Kl * distance + Kq * distance * distance);

    // Ambient Light
    float ambient = 0.45f;

    // Diffuse Light
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // Specular Light (Smoother, Softer Highlights)
    float specularLight = 1.0;
    vec3 viewDirection = normalize(cameraPosition - currentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0f), 16);  // Smoother reflection
    float specular = specAmount * specularLight;


    // Final Color Output
    vec4 textureColor = texture(textures[TexIndex], textCoords);
    vec4 result = textureColor * lightColor * (diffuse + ambient + specular) * attenuation;
    
    // Add aura effect for hovered instances
    if (color.r == 1.0 && color.g == 1.0 && color.b == 0.0) {
        float auraStrength = 0.4;
        float normalEdge = 1.0 - max(dot(normalize(cameraPosition - currentPosition), normal), 0.0);
        vec4 auraColor = vec4(1.0, 1.0, 1.0, 0.5) * normalEdge * auraStrength;
        result += auraColor;
    }
    FragColor = result;
}
