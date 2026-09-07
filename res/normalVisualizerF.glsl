#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec4 vColor;

out vec4 outColor;

void main() {
	vec3 norm = normalize(vNormal);

	vec3 normalColor = norm * 0.5 + 0.5;

	outColor = vec4(normalColor, 1.0);
}
