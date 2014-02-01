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
#include <glm/gtc/matrix_transform.hpp>
RenderContext::RenderContext() {
	// TODO Auto-generated constructor stub

}

GLFWwindow* window = NULL;

void mainLoop();

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
	mainLoop();
	printf("all ok\n");
	return true;
}

void mainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glm::mat4 orthoMat = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glLoadMatrixf(&orthoMat[0][0]);
		//glOrtho();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}



RenderContext::~RenderContext() {
	glfwTerminate();
	if(window)
		glfwDestroyWindow(window);
}

