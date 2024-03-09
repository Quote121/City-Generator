#version 460 core

// Not using texture maps
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

#define MAX_NR_POINT_LIGHTS 50 // Max number of lights that can effect this object

in vec3 FragPos;
in vec3 Normal;

// Position of camera
uniform vec3 viewPos;

uniform Material material;

// Lighting
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_NR_POINT_LIGHTS];
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

    vec3 result;
    result = CalcDirLight(dirLight, norm, viewDir);
    
    // point lights
    for(int i = 0; i < NumValidPointLights && i < MAX_NR_POINT_LIGHTS; i++)
       result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 

    // Hacky fix
    // If there are no lighting effects applied then we show the modells default diffuse
    if (ShowLighting)
    {
        FragColor = vec4(result, 1.0);
    }
    else
    {
        FragColor = vec4(material.diffuse, 1.0);
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

    // No texture coords
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

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

    // No texture coords
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
