#version 330 core

in vec4 fragColor;
in vec2 texCoord;
out vec4 outColor;
uniform sampler2D outTexture;

void main(){
	outColor = texture(outTexture, texCoord) * fragColor;
}
