#version 330 core

in vec4 fragColor;
out vec4 outColor;

uniform vec3 u_AmbientColor;
uniform float u_AmbientStrength;

void main(){
	//vec3 ambient = fragColor.rgb * u_AmbientColor * u_AmbientStrength;
	vec3 ambient = u_AmbientStrength * u_AmbientColor;
	vec3 result = ambient * fragColor.rgb;
	outColor = vec4(result, fragColor.a);
}
