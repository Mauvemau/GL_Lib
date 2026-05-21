#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec4 vColor;
in vec2 vUV;

uniform sampler2D u_Texture;
uniform bool u_UseTexture;

struct DirLight {
	vec3 direction;
	vec3 color;
};

struct PointLight {
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 color;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;
};

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform DirLight u_dirLight;

#define NR_POINT_LIGHTS 6
uniform PointLight u_pointLights[NR_POINT_LIGHTS];

#define NR_SPOT_LIGHTS 6
uniform SpotLight u_spotLights[NR_SPOT_LIGHTS];

uniform Material u_material;
uniform vec3 u_viewPos;

out vec4 outColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// Directional light
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(norm, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

	vec3 ambient  = light.color * u_material.ambient;
	vec3 diffuse  = light.color * diff * u_material.diffuse;
	vec3 specular = light.color * spec * u_material.specular;

	return (ambient + diffuse + specular);
}

// Point light
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant +
	light.linear * distance +
	light.quadratic * distance * distance);

	vec3 ambient = vec3(0.0);
	vec3 diffuse = light.color * diff * u_material.diffuse;
	vec3 specular = light.color * spec * u_material.specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant +
	light.linear * distance +
	light.quadratic * distance * distance);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient = vec3(0.0);
	vec3 diffuse  = light.color * diff * u_material.diffuse;
	vec3 specular = light.color * spec * u_material.specular;

	ambient  *= attenuation * intensity;
	diffuse  *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}

void main() {
	vec3 norm = normalize(vNormal);
	vec3 viewDir = normalize(u_viewPos - vFragPos);
	vec4 baseColor = vColor;

	vec3 result = CalcDirLight(u_dirLight, norm, viewDir);

	for(int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += CalcPointLight(u_pointLights[i], norm, vFragPos, viewDir);
	}


	for(int i = 0; i < NR_SPOT_LIGHTS; i++) {
		result += CalcSpotLight(u_spotLights[i], norm, vFragPos, viewDir);
	}

	if (u_UseTexture) {
		baseColor *= texture(u_Texture, vUV);
	}

	result *= baseColor.rgb;

	outColor = vec4(result, baseColor.a);
}
