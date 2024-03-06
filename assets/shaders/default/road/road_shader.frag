#version 460 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define MAX_NR_POINT_LIGHTS 50 // Max number we can have

in vec3 FragPos;
in vec3 Normal;

uniform vec3 colour;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_NR_POINT_LIGHTS];
uniform Material material;
uniform int NumValidPointLights; // Max number there are
uniform bool ShowLighting;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: point lights

    for(int i = 0; i < NumValidPointLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 

    // Hacky fix
    // If there are no lighting effects applied then we show the modells default diffuse
    if (ShowLighting)
    {
        FragColor = vec4(result, 1.0);
    }
    else
    {
        FragColor = vec4(colour, 1.0);
    }

}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    
    // Texcoords
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    // vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    // vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    // No texture coords
    vec3 ambient = light.ambient * colour;
    vec3 diffuse = light.diffuse * diff * colour;
    vec3 specular = light.specular * spec * colour;


    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // // combine results Texture coords
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    // vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    // vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    // No texture coords
    vec3 ambient = light.ambient * colour;
    vec3 diffuse = light.diffuse * diff * colour;
    vec3 specular = light.specular * spec * colour;

    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
