#version 330 core

in vec2 textureCoord;

uniform sampler2D texUnit;

out vec4 color;

void main(){
	color = texture(texUnit,vec2(textureCoord));
}