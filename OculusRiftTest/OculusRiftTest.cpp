// OculusRiftTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <glfw3.h>
#include "Shader.h"
#include "Texture.h"
#include "headers/openvr.h"
#include <opencv2/cvconfig.h>

using namespace cv;

using namespace vr;

IVRSystem* vrSys;

void initVR() {
	EVRInitError eError = VRInitError_None;
	vrSys = VR_Init(&eError, VRApplication_Scene);

	if (eError == VRInitError_None) {
		printf("Failed to init SteamVR: %s\n", VR_GetVRInitErrorAsEnglishDescription(eError));
	}

	if (!VRCompositor()) {
		printf("Failed to start compositor\n");
	}
}


int main()
{

	

	int resultGLFW = glfwInit();

	glewExperimental = true;

	float vertices [] = {
		-1.0f,1.0f,
		-1.0f,-1.0f,
		1.0f,1.0f,
		1.0f,-1.0f
	};

	float texCoords[] = {
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,0.0f,
		1.0f,1.0f
	};

	int indices[] = {
		0,1,2,2,1,3
	};


	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	GLFWwindow* offscreen_context = glfwCreateWindow(1920, 1080, "OVR TEST", NULL, NULL);

	glfwMakeContextCurrent(offscreen_context);

	if (glewInit() != GLEW_OK) {
		printf("GLEW FAILED TO INIT\n");
	}

	Shader shaderProgram("vert.glsl", "frag.glsl");

	Texture webcamTexture;

	GLuint vao, vbo, ibo,vboTex;

	shaderProgram.useProgram();

	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &vboTex);

		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, vboTex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), &texCoords, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		glGenBuffers(1, &ibo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); 
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glUseProgram(0);

	
	VideoCapture videoCap("http://192.168.0.22:1181/?action=stream");

	Mat videoIn;

	Mat finalOut;

	Mat* finalImage = new Mat();

	initVR();

//	namedWindow("Test Camera", 1);

	
	videoCap.set(CAP_PROP_FRAME_WIDTH, 1920.0);
	videoCap.set(CAP_PROP_FRAME_HEIGHT, 1080.0);
	videoCap.set(CAP_PROP_FPS, 90.0);

	while (!glfwWindowShouldClose(offscreen_context)) {

		
		
		
		while (!videoCap.isOpened()) {
		//	printf("Waiting for stream\n");
		}

		videoCap >> videoIn;

		//imwrite("test.png", videoIn);

		cvtColor(videoIn, finalOut, COLOR_BGR2RGB);
		flip(finalOut, *finalImage, 0);

		TrackedDevicePose_t arrays;

		VRCompositor()->WaitGetPoses(&arrays, 1, &arrays, 1);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


		try {
			webcamTexture.setTextureDataCV(finalImage->ptr());
		}
		catch (Exception e) {
			printf(e.what()); 
		}

		shaderProgram.useProgram();

		glBindVertexArray(vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		webcamTexture.bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);

		Texture_t left = {
			(void*)(uintptr_t)webcamTexture.getTextureID(),
			TextureType_OpenGL,
			ColorSpace_Gamma
		};

		Texture_t right = {
			(void*)(uintptr_t)webcamTexture.getTextureID(),
			TextureType_OpenGL,
			ColorSpace_Gamma
		};

	

		VRCompositor()->Submit(Eye_Left, &left);
		VRCompositor()->Submit(Eye_Right, &right);

		//flip(finalOut, finalOut, 0);

	//	imwrite("testFinal.png", passingMat);

		webcamTexture.setTextureDataCV(finalOut.ptr());

		shaderProgram.useProgram();

		glBindVertexArray(vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		webcamTexture.bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(0);


		glfwSwapBuffers(offscreen_context);
		
		glfwPollEvents();
	}

    std::cout << "Hello World!\n"; 

	VR_Shutdown();
	
	glfwTerminate();
}





// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
