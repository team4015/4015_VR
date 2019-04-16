#pragma once
#include <fstream>

#include <string>
#include <GL/glew.h>
#include <iostream>


using namespace std;

class Shader
{
public:
	Shader(string filePathVert,string filePathFrag) {
		string codeVert = "";

		ifstream fileStream(filePathVert);

		string line = "";

		while (getline(fileStream, line)) {
			codeVert += line+"\n";
		}

		fileStream.close();

		setupVertexShader(codeVert);

		string codeFrag = "";

		ifstream fileStreamFrag(filePathFrag);

		line = "";

		while (getline(fileStreamFrag, line)) {
			codeFrag += line+"\n";
		}

		fileStreamFrag.close();

		setupFragShader(codeFrag);

		createProgram();


	};

	void useProgram() {
		glUseProgram(shaderProgramID);
	}

	~Shader();

private:
	GLuint vertShaderID;
	GLuint fragShaderID;
	GLuint shaderProgramID;

	void createProgram() {
		shaderProgramID = glCreateProgram();

		glAttachShader(shaderProgramID, vertShaderID);
		glAttachShader(shaderProgramID, fragShaderID);

		glLinkProgram(shaderProgramID);

		int success;

		glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);

		if (!success) {
			char info[512];
			glGetProgramInfoLog(shaderProgramID,512,NULL, info);
			cout << "Error compiling program: " << info << endl;
		}
	}

	void setupVertexShader(std::string code) {
		vertShaderID = glCreateShader(GL_VERTEX_SHADER);
		const char * codeGl = code.c_str();

		glShaderSource(vertShaderID, 1, &codeGl, NULL);

		glCompileShader(vertShaderID);

		int success;

		glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &success);

		if (!success) {
			char log[512];
			glGetShaderInfoLog(vertShaderID, 512, NULL, log);
			cout << "Error compiling vertex shader: " << log << endl;
			return;
		}

	}

	void setupFragShader(std::string code) {
		fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		const char * codeGl = code.c_str();

		glShaderSource(fragShaderID, 1, &codeGl, NULL);

		glCompileShader(fragShaderID);

		int success;

		glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &success);

		if (!success) {
			char log[512];
			glGetShaderInfoLog(fragShaderID, 512, NULL, log);
			cout << "Error compiling fragment shader: " << log << endl;
			return;
		}



	}
};

