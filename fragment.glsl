#version 330 core

out vec4 FragColour;

in vec3 ourColour;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float rOffset;
uniform float gOffset;
uniform float bOffset;

uniform float mixPercentage;

void main() {
	//FragColour = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixPercentage);
	FragColour = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixPercentage) * vec4(ourColour.r + rOffset, ourColour.g + gOffset, ourColour.b + bOffset, 1.0f);
}