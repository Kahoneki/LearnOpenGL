#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
out vec3 ourColour;

uniform float xOffset;

void main() {
	gl_Position = vec4(aPos.x + xOffset, aPos.y, aPos.z, 1.0);
	ourColour = aColour;
}