#version 330 core

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 fragColor;
out vec2 texCoord;

uniform mat4 u_MVP;

void main(){
	gl_Position = u_MVP * aPosition;
	fragColor = aColor;
	texCoord = aTexCoord;
}
