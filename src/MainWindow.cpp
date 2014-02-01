/*
 * MainWindow.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: adi
 */

#include "MainWindow.h"
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <cstdio>

GLFWwindow* window;

static void error_callback(int error, const char* description)
{
	fputs(description, stdout);
}

MainWindow::MainWindow()
{
}

void render();

bool MainWindow::run()
{
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if(window == NULL)
	{
		printf("failed to create window\n");
		return false;
	}
	glfwSetErrorCallback(error_callback);
	glfwMakeContextCurrent(window);
	render();
	return true;
}

void render()
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
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
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

MainWindow::~MainWindow()
{
	if(window)
		glfwDestroyWindow(window);
}

