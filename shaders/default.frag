#version 330 core

out vec4 FragColor;
in vec3 Normal;
in vec3 currentPosition;

uniform vec4 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - currentPosition);
    float distance = length(lightPosition - currentPosition);

    // Adjusted Attenuation to avoid extreme fading
    float Kc = 1.0;
    float Kl = 0.02;  // Lower attenuation
    float Kq = 0.004; // Lower attenuation
    float attenuation = 1.0 / (Kc + Kl * distance + Kq * distance * distance);

    // Ambient Light
    float ambient = 0.2f;

    // Diffuse Light
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // Specular Light (Smoother, Softer Highlights)
    float specularLight = 0.3;
    vec3 viewDirection = normalize(cameraPosition - currentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0f), 16);  // Smoother reflection
    float specular = specAmount * specularLight;

    // Final Color Output
    FragColor = lightColor * (diffuse + ambient + specular) * attenuation;
}
