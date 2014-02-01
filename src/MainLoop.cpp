/*
 * MainLoop.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: adi
 */

#include "MainLoop.h"
#include "glm/gtc/matrix_transform.hpp"
#include <lib3ds/file.h>
#include <cstdio>
MainLoop::MainLoop()
{
	Lib3dsFile* car = lib3ds_file_load("models/car.3ds");
	if(car != NULL)
	{
		printf("yay");
	}
	else
		printf("nay");

}

void MainLoop::run(GLFWwindow* window)
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

MainLoop::~MainLoop()
{
	// TODO Auto-generated destructor stub
}

