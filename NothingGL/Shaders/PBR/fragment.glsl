#version 330 core

in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;  

out vec4 fragColor;

#define PI 3.1415926

uniform sampler2D Albedo;
uniform sampler2D Metallic; //todo make specular and metallic work seperately, also give shininess to material
uniform sampler2D Roughness;
uniform sampler2D Normal;
uniform sampler2D AO;

struct directionalLight
{
    vec3 rotation;
    float power;
    vec3 color;
};

struct pointLight
{
    vec3 position;
    vec3 color;
    float power;
    float constant;
    float linear;
    float quadratic;  
};

uniform directionalLight sun;
uniform pointLight pLight;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 CalcDirLight(directionalLight light, vec3 normal, vec3 viewDir)
{
    //light direction and reflection
    vec3 lightDir = normalize(-light.rotation);
    vec3 reflectDir = reflect(-lightDir, normal);
    //specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = texture(Metallic, texCoord).x * spec * light.color; 
    //combine
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 lightStrength  = light.color * diff * light.power;
    
    return (lightStrength + specular);
}  

vec3 CalcPointLight(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //light direction and reflection
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    //specular shading
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = texture(Metallic, texCoord).x * spec * light.color;
    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 /   (light.constant
                                + light.linear * distance
                                + light.quadratic * (distance * distance));    
    //combine
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 lightStrength  = light.color  * diff * light.power;
    //attenuate
    lightStrength  *= attenuation;
    specular *= attenuation;

    return (lightStrength + specular);
}
//uh oh scary pbr

float NormalDistribution(vec3 N, vec3 H, float Rougness) //roughness calculation
{
    float NdotH = max(dot(N, H), 0.0);
    float denom = (NdotH*NdotH * (Rougness*Rougness - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return Rougness*Rougness / denom;
}
  
float GeometryOcclusion(vec3 N, vec3 V, vec3 L, float k) //geometry shading 
{
    float ggx1 = (max(dot(N, V), 0.0)) / (max(dot(N, V), 0.0)) * (1.0 - k) + k;
    float ggx2 = (max(dot(N, L), 0.0)) / (max(dot(N, L), 0.0)) * (1.0 - k) + k;
	
    return ggx1 * ggx2;
}

vec3 Fresnel(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}


void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 sunLight = CalcDirLight(sun, norm, viewDir); 

    int lightCount = 1;
    vec3 pointLightTotal = vec3(0.0);
    for(int i = 0; i < lightCount; i++)
    {
        pointLightTotal += CalcPointLight(pLight, norm, fragPos, viewDir);
    }
    
    fragColor = vec4(pointLightTotal + sunLight, 1.0) * texture(Albedo, texCoord);
}