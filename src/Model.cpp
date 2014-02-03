/*
 * Model.cpp
 *
 *  Created on: Feb 2, 2014
 *      Author: adi
 */

#include "Model.h"
#include "Shader/ShaderProgram.hpp"
#include <lib3ds/file.h>
#include <lib3ds/mesh.h>
#include <string.h>

Model::Model() : total_num_faces(0), vao_id_(0), vbo_id_(0)
{
}

bool Model::LoadModel(std::string file,ShaderProgram& shader)
{
	if(vao_id_)
		return false;
	Lib3dsFile* model3ds = lib3ds_file_load(file.c_str());
	if(!model3ds)
		return false;

	for(Lib3dsMesh* mesh = model3ds->meshes; mesh; mesh = mesh->next)
	{
		total_num_faces+= mesh->faces;
	}
	Lib3dsVector * vertices = new Lib3dsVector[total_num_faces * 3];
	total_num_faces = 0;
	for(Lib3dsMesh* mesh = model3ds->meshes; mesh; mesh = mesh->next)
	{
		for(uint32_t face_cursor=0; face_cursor < mesh->faces; face_cursor++)
		{
			for(int i=0; i<3; i++)
			{
				//copy mesh vertice data to array
				memcpy(& vertices[total_num_faces * 3 + i], mesh->pointL[mesh->faceL[face_cursor].points[i]].pos, sizeof(Lib3dsVector));
			}
			total_num_faces++;
		}
	}

	//the vao contains the states needed to draw aka what attrib are bind to what vbos
	glGenVertexArrays(1, &vao_id_);
	glBindVertexArray(vao_id_);
	{

		glGenBuffers(1, &vbo_id_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id_); //GL_ARRAY_BUFFER means array of vertex data
		glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector)*3*total_num_faces , vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(shader.attribute("inPos"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(shader.attribute("inPos"));

		//		glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
		//		glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector)*3*faceCount, normals, GL_STATIC_DRAW);
		//		glVertexAttribPointer(shader.attribute("inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		//		glEnableVertexAttribArray(shader.attribute("inNormal"));

	}
	glBindVertexArray(0);

	delete vertices;
	return true;
}

void Model::Draw()
{
	glBindVertexArray(vao_id_);
	glDrawArrays(GL_TRIANGLES, 0, total_num_faces * 3 );
	glBindVertexArray(0);
}

Model::~Model()
{
	if(vao_id_)
		glDeleteVertexArrays(1, &vao_id_);

	if(vbo_id_)
		glDeleteBuffers(1, &vbo_id_);
}

