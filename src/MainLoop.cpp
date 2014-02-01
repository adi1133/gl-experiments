/*
 * MainLoop.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: adi
 */


#include <GL/glew.h>
#include "MainLoop.h"
#include "glm/gtc/matrix_transform.hpp"
#include <lib3ds/file.h>
#include <lib3ds/mesh.h>
#include <cstdio>
#include <string.h>
uint32_t faceCount = 0;

MainLoop::MainLoop()
{
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

		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector)*3*faceCount, vertices, GL_STATIC_DRAW);

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
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//	glm::mat4 orthoMat = glm::ortho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glm::mat4 orthoMat = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);
		glLoadMatrixf(&orthoMat[0][0]);
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


		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 1);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawArrays(GL_TRIANGLES, 0, faceCount* 3);
		glDisableClientState(GL_VERTEX_ARRAY);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

MainLoop::~MainLoop()
{
	// TODO Auto-generated destructor stub
}

