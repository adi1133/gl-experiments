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
#include "glm.h"

Model::Model() : total_num_faces(0), vao_id_(0), vert_pos_id_(0),vert_norm_id_(0), ibo_id_(0)
{
}

bool Model::LoadModel(std::string file,ShaderProgram& shader)
{

	if(vao_id_)
		return false;
	Lib3dsFile* model3ds = lib3ds_file_load(file.c_str());
	if(!model3ds)
		return false;
	uint32_t verts = 0;


	for(Lib3dsMesh* mesh = model3ds->meshes; mesh; mesh = mesh->next)
	{
		verts+= mesh->points;
		total_num_faces+= mesh->faces;
	}

	glGenVertexArrays(1, &vao_id_); //generate vertex array object to hold draw state
	glBindVertexArray(vao_id_);
	{
		glGenBuffers(1, &vert_pos_id_);//generate buffer handle
		glBindBuffer(GL_ARRAY_BUFFER, vert_pos_id_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsPoint) * verts , NULL, GL_STATIC_DRAW); //allocate memory on GPU
		glVertexAttribPointer(shader.attribute("inPos"), 3, GL_FLOAT, GL_FALSE, 0, 0);   //bind to shader attribute input
		glEnableVertexAttribArray(shader.attribute("inPos"));							//activate shader attribute input

		glm::vec3* normals = new glm::vec3[verts]; //default contructor is vec3(0)

		glGenBuffers(1, &ibo_id_);//generate buffer handle
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id_); //attach handle to current GL_ELEMENT_ARRAY_BUFFER(holds vertex indexes)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, total_num_faces * 3 * sizeof(Lib3dsWord) , NULL, GL_STATIC_DRAW);//allocate memory on GPU

		verts = 0;
		total_num_faces = 0;

		for(Lib3dsMesh* mesh = model3ds->meshes; mesh; mesh = mesh->next)
		{
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(Lib3dsPoint) * verts, sizeof(Lib3dsPoint) * mesh->points, mesh->pointL );
			for(uint32_t face_cursor=0; face_cursor < mesh->faces; face_cursor++)
			{
				Lib3dsFace * currentFace = & mesh->faceL[face_cursor];
				glm::vec3 faceNormal = glm::vec3(currentFace->normal[0], currentFace->normal[1], currentFace->normal[2]);
				faceNormal = glm::normalize(faceNormal);
				GLushort index[3];
				for(int i = 0; i<3; i++)
				{
					index[i] = currentFace->points[i] + verts;
					normals[index[i]] += faceNormal;
				}
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, total_num_faces * sizeof(index), sizeof(index), index);
				total_num_faces++;
			}
			verts+= mesh->points;
		}

		for(uint32_t i; i < verts; i++)
		{
			normals[i] = glm::normalize(normals[i]);
		}
		glGenBuffers(1, &vert_norm_id_);
		glBindBuffer(GL_ARRAY_BUFFER, vert_norm_id_);
		glBufferData(GL_ARRAY_BUFFER, verts * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
		glVertexAttribPointer(shader.attribute("inNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);   //bind to shader attribute input
		glEnableVertexAttribArray(shader.attribute("inNormal"));							//activate shader attribute input

		delete normals;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//unbind the current buffer object since the vao already knows what to use from glEnableVertexAttribArray

		/*
		 * the vao contains:
		 * currently bound buffers(one of each type) - here the index buffer is important
		 * what shaders attributes are active
		 * what array buffers are bound to what shader attributes
		 */
	}
	glBindVertexArray(0);

	return true;
}

void Model::Draw()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(vao_id_);
	glDrawElements(GL_TRIANGLES, total_num_faces * 3, GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
}

Model::~Model()
{
	if(vao_id_)
		glDeleteVertexArrays(1, &vao_id_);
	if(vert_pos_id_)
		glDeleteBuffers(1, &vert_pos_id_);
	if(vert_norm_id_)
		glDeleteBuffers(1, &vert_norm_id_);
	if(ibo_id_)
		glDeleteBuffers(1, &ibo_id_);
}

