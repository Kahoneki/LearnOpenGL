#version 330 core

in vec3 fragPos;
in vec3 normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
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


	//-------------------------//
	//----SPECULAR LIGHTING----//
	//-------------------------//
	
	float specularStrength = 0.5; //How bright the specular highlight is
	int shininess = 32; //How shiny the specular highlight is
	
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, unitNormal); //reflect() expects the first argument to point from the light source towards the fragment position, so we have to negate lightDir.

	float specularAmount = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * specularAmount * lightColour;

	
	//-------------------------------//
	//----CALCULATING FINAL PHONG----//
	//-------------------------------//
	
	vec3 result = (ambient + diffuse + specular) * objectColour;

	FragColour = vec4(result, 1.0);
}