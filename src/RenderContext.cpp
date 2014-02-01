/*
 * RenderContext.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: adi
 */

#include "RenderContext.h"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include "MainWindow.h"
RenderContext::RenderContext() {
	// TODO Auto-generated constructor stub

}

bool runWindow();

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
		printf("all ok\n");
		return runWindow();
}

bool runWindow()
{
	MainWindow window;
	return window.run();
}



RenderContext::~RenderContext() {
	glfwTerminate();
}

