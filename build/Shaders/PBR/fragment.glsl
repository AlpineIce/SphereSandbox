#version 400 core

in vec2 texCoord;
in vec3 fragPos;  
in mat3 TBN;

out vec4 fragColor;

#define PI 3.1415926
#define MAX_POINT_LIGHTS 4

uniform sampler2D Albedo;
uniform sampler2D Metallic; //todo make specular and metallic work seperately, also give shininess to material
uniform sampler2D Roughness;
uniform sampler2D NormalMap;
uniform sampler2D AO;

struct Light
{
    vec3 pos;
    float power;
    vec3 color;
};

uniform vec3 ambientLight;
uniform Light sun;
uniform Light[4] pLights;
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

vec3 CalcLight(Light light, vec3 normal, vec3 lightDir, vec3 viewDir, float F0)
{
    //light direction
    vec3 H = normalize(viewDir + lightDir);

    //roughness
    float NDF = NormalDistribution(normal, H, texture(Roughness, texCoord).x);       
    float G = GeometryOcclusion(normal, viewDir, lightDir, texture(Roughness, texCoord).x);

    //specular
    float kS = F0 + (1.0 - F0) * pow(1.0 - max(dot(H, viewDir), 0.0), 5.0);
    float kD = 1.0 - kS;
    kD *= 1.0 - texture(Metallic, texCoord).x;	 

    float numerator = NDF * G * kS;
    float denominator = 8.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0)  + 0.0001;
    float specular = numerator / denominator;  

    float NdotL = max(dot(normal, lightDir), 0.0);

    return ((kD / PI + specular) * light.color * NdotL);
}

void main()
{
    //normal mapping
    vec3 normal = texture(NormalMap, texCoord).xyz * 2.0 - 1.0;
    normal = normalize(TBN * normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    //surface reflection at zero incidence
    float F0 = 0.04; 
    float avgAlbedo = (texture(Albedo, texCoord).x + texture(Albedo, texCoord).y + texture(Albedo, texCoord).z) / 3.0;
    F0 = mix(F0, avgAlbedo, texture(Metallic, texCoord).x);

    vec3 sunLight = CalcLight(sun, normal, normalize(sun.pos), viewDir, F0) * sun.power; 
    vec3 pointLightTotal = vec3(0.0);
    for(int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        float distance = length(pLights[i].pos - fragPos);
        float falloff = 1.0 / pLights[i].power;
        float attenuation = max(1.0 / (distance + 1.0) * pLights[i].power, 1.0);
        pointLightTotal += CalcLight(pLights[i], normal, normalize(pLights[i].pos - fragPos), viewDir, F0) * attenuation;
    }
    
    
    vec3 totalLight = pointLightTotal + sunLight;
    float gamma = 2.2;

    fragColor = vec4(pow(totalLight, vec3(gamma)) + ambientLight, 1.0) * texture(Albedo, texCoord);
}