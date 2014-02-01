//============================================================================
// Name        : GLSkeleton.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <cstdio>
#include <cstdlib>
#include <GLFW/glfw3.h>
using namespace std;

int main() {
	//printf("DISPLAY=%s\n", getenv("DISPLAY"));

	if (!glfwInit())
	{
		printf("glfwInit() failed");
		return -1;
	}
	printf("all ok");
	glfwTerminate();
	return 0;
}
