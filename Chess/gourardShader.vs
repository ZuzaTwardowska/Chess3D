#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec4 FragColor;

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool isBoard;

uniform vec3 viewPos;
uniform Material material;
uniform SpotLight spotLight[3];

uniform sampler2D texture_diffuse1;

uniform bool isBlinn;
uniform float fogIntensity;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec2 TexCoords);
float CalcFog(vec3 FragPos);

void main()
{
    vec3 FragPos = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
    vec2 TexCoords = aTexCoords;    
    if(isBoard)
    {
        gl_Position = projection * view * model * vec4(FragPos, 1.0);
    }
    else
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
    vec3 result = CalcSpotLight(spotLight[0], Normal, aPos, TexCoords);
    for(int i = 1; i < 3; i++)
        result += CalcSpotLight(spotLight[i], Normal, aPos, TexCoords);  
    
    float fog = CalcFog(aPos);
    result = mix(vec3(0.05f), result, fog);
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec2 TexCoords)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);
    // diffuse shading
    float diff = max(dot(normalize(normal), lightDir), 0.0);
    // specular shading
    float spec = 0.0f;
    if(!isBlinn)
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    else
    {
        vec3 halfDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normalize(normal), halfDir), 0.0), 32.0);
    }
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

float CalcFog(vec3 FragPos) {
    if (fogIntensity == 0) return 1;
    float gradient = (fogIntensity * fogIntensity - 50 * fogIntensity + 60);
    float distance = length(viewPos - FragPos);
    float fog = exp(-pow((distance / gradient), 5));
    fog = clamp(fog, 0.0, 1.0);
    return fog;
}
