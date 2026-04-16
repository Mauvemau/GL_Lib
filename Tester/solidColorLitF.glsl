#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec4 vColor;

uniform vec3 u_AmbientColor;
uniform float u_AmbientStrength;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;

out vec4 outColor;

void main(){
	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(u_LightPos - vFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_LightColor;

	vec3 ambient = vColor.rgb * u_AmbientColor * u_AmbientStrength;
	vec3 result = ambient + diffuse;
	outColor = vec4(result, vColor.a);
}
