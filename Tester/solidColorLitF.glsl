#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec4 vColor;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material u_material;

uniform vec3 u_AmbientColor;
uniform float u_AmbientStrength;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

uniform vec3 u_ViewPos;
uniform float u_SpecularStrength;

out vec4 outColor;

void main(){
	// ambient
	vec3 ambient = u_LightColor * u_material.ambient;

	//diffuse
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(u_LightPos - vFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = u_LightColor * (diff * u_material.diffuse);

	// Specular
	vec3 viewDir = normalize(u_ViewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	vec3 specular = u_LightColor * (spec * u_material.specular);

	vec3 result = (ambient + diffuse + specular);
	outColor = vec4(result, vColor.a);
}
