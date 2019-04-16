#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoord;

out vec2 textureCoord;

void main(){
	gl_Position = vec4(pos,0.0f,1.0f);
	textureCoord = texCoord;
}