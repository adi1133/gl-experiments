/*
 * RenderContext.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: adi
 */

#include "RenderContext.h"
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include "MainLoop.h"
RenderContext::RenderContext() {
}

GLFWwindow* window = NULL;


static void error_callback(int error, const char* description)
{
	fputs(description, stdout);
}

bool RenderContext::init() {
	if(getenv("DISPLAY") == NULL)
	{
		printf("missing DISPLAY global var, this should be :0 ?\n");
		return false;
	}

	if (!glfwInit())
	{
		printf("glfwInit() failed\n");
		return false;
	}

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if(window == NULL)
	{
		printf("failed to create window\n");
		return false;
	}
	glfwSetErrorCallback(error_callback);
	glfwMakeContextCurrent(window);
	if (GLEW_OK != glewInit())
	{
		printf("glewInit() failed\n");
		return false;
	}

	MainLoop mainLoop;
	mainLoop.run(window);

	printf("all ok\n");
	return true;
}



RenderContext::~RenderContext() {
	glfwTerminate();
	if(window)
		glfwDestroyWindow(window);
}

