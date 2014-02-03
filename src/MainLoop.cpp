/*
 * MainLoop.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: adi
 */


#include <GL/glew.h>
#include "MainLoop.h"
#include "glm.h"
#include <cstdio>
#include <string.h>
#include "Shader/ShaderProgram.hpp"

uint32_t faceCount = 0;


GLuint vaoID[1];
ShaderProgram shader;

int scrollPos = 0;


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if( (key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	scrollPos+= yoffset;
}

MainLoop::MainLoop(GLFWwindow* window) : window(window)
{

	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);
	Shader vertexShader;
	vertexShader.loadFromFile(GL_VERTEX_SHADER, "Shaders/simple.vert");


	Shader fragmentShader;
	fragmentShader.loadFromFile(GL_FRAGMENT_SHADER, "Shaders/simple.frag");


	shader.attachShader(vertexShader);
	shader.attachShader(fragmentShader);
	shader.linkProgram();


	monkey_.LoadModel("models/car.3ds", shader);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


}




void MainLoop::run()
{
	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		//glm::mat4 model = glm::scale(glm::mat4(1), glm::vec3(0.5));
		glm::mat4 model(1);
		//model = glm::scale(model, glm::vec3(zoom));
		model = glm::scale(model, glm::vec3(0.0005,0.0005,0.0005));
		glm::mat4 view = glm::lookAt(glm::vec3(1, 0, scrollPos * 0.1 ), glm::vec3(0,0,0), glm::vec3(0,1,0));

		//glm::mat4 proj = glm::ortho(-ratio * 5, ratio * 5, -5.0f, 5.0f, -5.0f, 5.0f);
		glm::mat4 proj = glm::perspective(90.0f, (float)width/height, 0.1f, 1000.0f);

		glUniformMatrix4fv(shader.uniform("tMat"), 1, GL_FALSE, glm::value_ptr(proj * view * model));

		shader.use();
		monkey_.Draw();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

MainLoop::~MainLoop()
{
}

