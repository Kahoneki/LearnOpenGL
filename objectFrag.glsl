#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;		//For point lights
	vec3 direction;		//For directional lights

	//Strengths
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//Attenuation terms
	float constant;
	float linear;
	float quadratic;
};

in vec2 TexCoords;
in vec3 fragPos;
in vec3 normal;

uniform Material material;
uniform Light light;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColour;

void main()
{

	float distanceLightTravelled = length(light.position - fragPos);
	
	float attenuation = 1.0 / (light.constant +
							   light.linear * distanceLightTravelled +
							   light.quadratic * (distanceLightTravelled * distanceLightTravelled));


	//------------------------//
	//----AMBIENT LIGHTING----//
	//------------------------//

	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
	ambient *= attenuation;
	
	
	//------------------------//
	//----DIFFUSE LIGHTING----//
	//------------------------//

	vec3 unitNormal = normalize(normal);
	vec3 lightDir = normalize(-light.direction);
	
	float angle = max(dot(unitNormal, lightDir), 0.0); //If angle > 90, dot product will be negative; negative colours are undefined, so max() ensures result will always be positive
	vec3 diffuse = light.diffuse * angle * vec3(texture(material.diffuse, TexCoords));
	diffuse *= attenuation;


	//-------------------------//
	//----SPECULAR LIGHTING----//
	//-------------------------//
	
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, unitNormal); //reflect() expects the first argument to point from the light source towards the fragment position, so we have to negate lightDir.

	float specularAmount = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specularAmount * vec3(texture(material.specular, TexCoords));
	specular *= attenuation;

	
	//-------------------------------//
	//----CALCULATING FINAL PHONG----//
	//-------------------------------//
	
	vec3 result = ambient + diffuse + specular;

	FragColour = vec4(result, 1.0);
}