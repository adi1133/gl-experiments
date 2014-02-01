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
ShaderProgram* shader = NULL;

MainLoop::MainLoop()
{
	Shader vertexShader(GL_VERTEX_SHADER);
	vertexShader.loadFromFile("Shaders/simple.vert");
	vertexShader.compile();


	Shader fragmentShader(GL_FRAGMENT_SHADER);
	fragmentShader.loadFromFile("Shaders/simple.frag");
	fragmentShader.compile();

	shader = new ShaderProgram();
	shader->attachShader(vertexShader);
	shader->attachShader(fragmentShader);
	shader->linkProgram();
	shader->addAttribute("inPos");
	shader->addUniform("tMat");

	Lib3dsFile* car = lib3ds_file_load("models/monkey.3ds");

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
			GLuint buffer;
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector)*3*faceCount, vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(shader->attribute("inPos"), 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(shader->attribute("inPos"));
		}
		glBindVertexArray(0);

		delete vertices;
		delete normals;
		printf("yay");
	}
	else
		printf("nay");

}

void MainLoop::run(GLFWwindow* window)
{
	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->use();
		glUniformMatrix4fv(shader->uniform("tMat"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
		glBindVertexArray(vaoID[0]);
		glDrawArrays(GL_TRIANGLES, 0, faceCount * 3 );
		glBindVertexArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

MainLoop::~MainLoop()
{
	if(shader)
		delete shader;
}

