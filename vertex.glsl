#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColour;
out vec2 TexCoord;

uniform float xOffset;
uniform float yOffset;
uniform mat4 rotationTransform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * rotationTransform * vec4(aPos.x + xOffset, aPos.y + yOffset, aPos.z, 1.0f);
	ourColour = aColour;
	TexCoord = aTexCoord;
}