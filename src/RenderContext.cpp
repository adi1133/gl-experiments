/*
 * RenderContext.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: adi
 */

#include "RenderContext.h"
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <GL/gl.h>
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

	bool opengl3core = true;
	if(opengl3core)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if(window == NULL)
	{
		printf("failed to create window\n");
		return false;
	}
	glfwSetErrorCallback(error_callback);
	glfwMakeContextCurrent(window);
	printf("%s\n", glGetString(GL_VERSION));

	glewExperimental = true;
	//http://www.opengl.org/wiki/OpenGL_Loading_Library
	//http://stackoverflow.com/questions/8302625
	if (GLEW_OK != glewInit())
	{
		printf("glewInit() failed\n");
		return false;
	}

	MainLoop mainLoop(window);
	mainLoop.run();

	printf("all ok\n");
	return true;
}



RenderContext::~RenderContext() {
	if(window)
		glfwDestroyWindow(window);
	glfwTerminate();
}

