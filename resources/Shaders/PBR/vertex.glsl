#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangents;
layout (location = 4) in vec3 aBiTtangents;

out vec3 fragPos;
out vec2 texCoord;
out mat3 TBN;

uniform mat4 projection;
uniform mat3 transposeMat;
uniform mat4 view;
uniform mat4 model;


void main()
{
    vec3 T = normalize(aTangents); //tangents
    vec3 B = normalize(aBiTtangents); //bitangents
    vec3 N = normalize(aNormal); //normals
    TBN = transposeMat * mat3(T, B, N);

    texCoord = aTexCoord;
    fragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}