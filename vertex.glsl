#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform float xOffset;
uniform float yOffset;
uniform mat4 transform;

out vec3 ourColour;

void main() {
	gl_Position =  vec4(aPos.x + xOffset, aPos.y + yOffset, aPos.z, 1.0f) * transform;
	ourColour = aColour;
	TexCoord = aTexCoord;
}