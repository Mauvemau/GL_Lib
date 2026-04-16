#version 330 core

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 vFragPos;
out vec3 vNormal;
out vec4 vColor;

void main(){
	gl_Position = u_Projection * u_View * u_Model * aPosition;
	vFragPos = vec3(u_Model * aPosition);

	vNormal = mat3(u_Model) * aNormal;
	vColor = aColor;
}
