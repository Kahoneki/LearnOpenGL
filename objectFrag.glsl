#version 330 core

out vec4 FragColour;

uniform vec3 objectColour;
uniform vec3 lightColour;

void main()
{
	float ambientStrength  = 0.1f;
	vec3 ambient = ambientStrength * lightColour;
	
	vec3 result = ambient * objectColour;
	FragColour = vec4(result, 1.0);
}