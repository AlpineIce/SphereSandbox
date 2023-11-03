#version 330 core
in vec4 vertexColor;
in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D tex0;
//uniform sampler2D tex1;
//uniform sampler2D tex2;
//uniform sampler2D tex3;
//uniform sampler2D tex4;
//uniform sampler2D tex5;
//uniform sampler2D tex6;
//uniform sampler2D tex7;

void main()
{
    fragColor = texture(tex0, texCoord);
}