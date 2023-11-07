#version 400 core

in vec3 normal;
in vec2 texCoord;
in vec3 fragPos;  

out vec4 fragColor;

#define PI 3.1415926
#define MAX_POINT_LIGHTS 32

uniform sampler2D Albedo;
uniform sampler2D Metallic; //todo make specular and metallic work seperately, also give shininess to material
uniform sampler2D Roughness;
uniform sampler2D Normal;
uniform sampler2D AO;

struct directionalLight
{
    vec3 rot;
    float power;
    vec3 color;
};

struct pointLight
{
    vec3 pos;
    vec3 color;
    float power;
    float constant;
    float linear;
    float quad;  
};

uniform directionalLight sun;
uniform pointLight[4] pLights;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 viewPos;

float NormalDistribution(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometryOcclusion(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 CalcDirLight(directionalLight light, vec3 normal, vec3 viewDir, vec3 F0)
{
    //light direction and reflection
    vec3 lightDir = normalize(light.rot);
    vec3 H = normalize(viewDir + lightDir);
    vec3 radiance = light.color * light.power;

    float NDF = NormalDistribution(normal, H, texture(Roughness, texCoord).x);       
    float G = GeometryOcclusion(normal, viewDir, lightDir, texture(Roughness, texCoord).x);
    vec3 fresnel = vec3(F0.x) + (1.0 - F0) * pow(1.0 - max(dot(H, viewDir), 0.0), 5.0);       
        
    vec3 kS = fresnel;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - texture(Metallic, texCoord).x;	 

    vec3 numerator = NDF * G * fresnel;
    float denominator = 8.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0)  + 0.0001;
    vec3 specular = numerator / denominator;  

    float NdotL = max(dot(normal, lightDir), 0.0);

    return ((kD * texture(Albedo, texCoord).xyz / PI + specular) * radiance * NdotL);
}  

vec3 CalcPointLight(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 F0)
{
    //light direction and reflection
    vec3 lightDir = normalize(light.pos - fragPos);
    vec3 H = normalize(viewDir + lightDir);
    float distance = length(light.pos - fragPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = light.color * attenuation * light.power;

    float NDF = NormalDistribution(normal, H, texture(Roughness, texCoord).x);       
    float G = GeometryOcclusion(normal, viewDir, lightDir, texture(Roughness, texCoord).x);
    vec3 fresnel = vec3(F0.x) + (1.0 - F0) * pow(1.0 - max(dot(H, viewDir), 0.0), 5.0);       
        
    vec3 kS = fresnel;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - texture(Metallic, texCoord).x;	 

    vec3 numerator = NDF * G * fresnel;
    float denominator = 8.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0)  + 0.0001;
    vec3 specular = numerator / denominator;  

    float NdotL = max(dot(normal, lightDir), 0.0);

    return ((kD * texture(Albedo, texCoord).xyz / PI + specular) * radiance * NdotL);
}

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    //surface reflection at zero incidence
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, texture(Albedo, texCoord).xyz, texture(Metallic, texCoord).xyz);

    vec3 sunLight = CalcDirLight(sun, norm, viewDir, F0); 

    int lightCount = 1;
    vec3 pointLightTotal = vec3(0.0);
    for(int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        pointLightTotal += CalcPointLight(pLights[i], norm, fragPos, viewDir, F0);
    }
    vec3 totalLight = pointLightTotal + sunLight;

    fragColor = vec4(totalLight, 1.0);

    //vec3 color = (pointLightTotal + sunLight) / ((pointLightTotal + sunLight) + vec3(2.0));
    //vec3 color = pow((pointLightTotal + sunLight), vec3(1.0/2.2));
}