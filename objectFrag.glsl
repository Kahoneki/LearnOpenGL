#version 330 core

in vec3 fragPos;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 objectColour;
uniform vec3 lightColour;

out vec4 FragColour;

void main()
{
	//------------------------//
	//----AMBIENT LIGHTING----//
	//------------------------//

	float ambientStrength  = 0.1f;
	vec3 ambient = ambientStrength * lightColour;
	
	
	//------------------------//
	//----DIFFUSE LIGHTING----//
	//------------------------//

	vec3 unitNormal = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	
	float angle = max(dot(unitNormal, lightDir), 0.0); //If angle > 90, dot product will be negative; negative colours are undefined, so max() ensures result will always be positive
	vec3 diffuse = angle * lightColour;

	
	//--------------------------//
	//----CALCULATING RESULT----//
	//--------------------------//
	
	vec3 result = (ambient + diffuse) * objectColour;

	FragColour = vec4(result, 1.0);
}