#pragma once
#include <GL/glew.h>
#include <opencv2/opencv.hpp>

class Texture
{
public:
	Texture() {
		glGenTextures(1, &textureID);
		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		unbind();


	}

	GLuint getTextureID() {
		return this->textureID;
	}


	void setTextureDataCV(uchar* data) {
		bind();

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1920, 1080, GL_RGB, GL_UNSIGNED_BYTE, data);

		unbind();
	}

	~Texture();

	void bind() {
		glBindTexture(GL_TEXTURE_2D, textureID);
	};
	static void unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	};

private:
	GLuint textureID;
	
};

