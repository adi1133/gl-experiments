/*
 * MainLoop.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: adi
 */


#include <GL/glew.h>
#include "MainLoop.h"
#include "glm.h"
#include <lib3ds/file.h>
#include <lib3ds/mesh.h>
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


	Lib3dsFile* car = lib3ds_file_load("models/car.3ds");

	if(car != NULL)
	{

		for(Lib3dsMesh* mesh = car->meshes; mesh; mesh = mesh->next)
		{
			faceCount+= mesh->faces;
		}

		Lib3dsVector * vertices = new Lib3dsVector[faceCount * 3];
		Lib3dsVector * normals = new Lib3dsVector[faceCount * 3];
		faceCount = 0;
		for(Lib3dsMesh* mesh = car->meshes; mesh; mesh = mesh->next)
		{
			lib3ds_mesh_calculate_normals(mesh, &normals[faceCount * 3]);
			for(uint32_t facec=0; facec < mesh->faces; facec++)
			{
				//Lib3dsFace* face = mesh->faceL
				for(int i=0; i<3; i++)
				{
					memcpy(& vertices[faceCount * 3 + i], mesh->pointL[mesh->faceL[facec].points[i]].pos, sizeof(Lib3dsVector));
				}
				faceCount++;
			}
		}



		glGenVertexArrays(1,&vaoID[0]);

		glBindVertexArray(vaoID[0]);
		{
			GLuint buffer[2];
			glGenBuffers(2, buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector)*3*faceCount, vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(shader.attribute("inPos"), 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(shader.attribute("inPos"));

			glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector)*3*faceCount, normals, GL_STATIC_DRAW);
			glVertexAttribPointer(shader.attribute("inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(shader.attribute("inNormal"));
		}
		glBindVertexArray(0);

		delete vertices;
		delete normals;
		printf("yay");

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}
	else
		printf("nay");

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
		glBindVertexArray(vaoID[0]);
		glDrawArrays(GL_TRIANGLES, 0, faceCount * 3 );
		glBindVertexArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

MainLoop::~MainLoop()
{
}

