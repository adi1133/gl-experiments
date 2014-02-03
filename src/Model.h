/*
 * Model.h
 *
 *  Created on: Feb 2, 2014
 *      Author: adi
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <string>
#include <GL/glew.h>

class ShaderProgram;
class Model
{
public:
	Model();
	bool LoadModel(std::string file, ShaderProgram& shader);
	void Draw();
	virtual ~Model();
private:
	uint32_t total_num_faces;
	GLuint vao_id_;
	GLuint vert_pos_id_;
	GLuint vert_norm_id_;
	GLuint ibo_id_;



};

#endif /* MODEL_H_ */
