#version 330 core

in vec2 texCoord;

uniform sampler2D texUnit;

out vec4 color;

void main(){
	color = texture(texUnit,texCoord);
}