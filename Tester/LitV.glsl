#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;
layout (location = 3) in vec2 aUV;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 vFragPos;
out vec3 vNormal;
out vec4 vColor;
out vec2 vUV;

void main(){
	gl_Position = u_Projection * u_View * u_Model * vec4(aPosition, 1.0);

	vFragPos = vec3(u_Model * vec4(aPosition, 1.0));

	mat3 normalMatrix = transpose(inverse(mat3(u_Model)));
	vNormal = normalMatrix * aNormal;

	vColor = aColor;
	vUV = aUV;
}
