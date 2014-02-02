#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <iostream>
#include <map>

#include "Shader.hpp"

using std::map;
using std::string;
using std::cout;
using std::endl;


class ShaderProgram
{
private:
	GLuint id;   // The unique ID / handle for the shader program

	// Map of uniforms and their binding locations
	map<string,GLint> uniformLocList;

public:

	//prevent copy operators since the two instances get destroyed twice
	ShaderProgram() : id(0){}
	ShaderProgram(const ShaderProgram& src) = delete;
	ShaderProgram & operator=(const ShaderProgram&) = delete;



	// Destructor
	~ShaderProgram()
	{
		// Delete the shader program from the graphics card memory to
		// free all the resources it's been using
		if(id)
			glDeleteProgram(id);
	}


	// Method to attach a shader to the shader program
	void attachShader(const Shader& shader)
	{
		if(!id)
			id = glCreateProgram();
		// Attach the shader to the program
		// Note: We identify the shader by its unique Id value
		glAttachShader( id, shader.getId() );
	}

	// Method to link the shader program and display the link status
	bool linkProgram()
	{
		// Perform the linking process
		glLinkProgram(id);

		// Check the status
		GLint linkStatus;
		glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
		if (GL_LINK_STATUS == GL_FALSE)
		{
			cout << "Shader program linking failed." << endl;
		}
		else
		{
			cout << "Shader program linking OK." << endl;
		}
		GLint length;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

		if(length)
		{
			GLchar log[length];
			glGetProgramInfoLog(id, length, NULL, log);
			cout << log << endl;
		}

		return linkStatus;

	}


	// Method to enable the shader program
	void use()
	{
		glUseProgram(id);
	}


	// Method to disable the shader program
	void disable()
	{
		glUseProgram(0);
	}


	// Returns the bound location of a named attribute
	GLint attribute(const string attribute)
	{
		GLint attrAddr= glGetAttribLocation( id, attribute.c_str() );
		if(attrAddr == -1)
		{
			cout << "Could not find attribute in shader program: " << attribute << endl;
			exit(-1);
		}
		return attrAddr;
	}


	// Method to returns the bound location of a named uniform
	GLint uniform(const string uniform)
	{
		// Note: You could do this method with the single line:
		//
		// 		return uniformLocList[uniform];
		//
		// But we're not doing that. Explanation in the attribute() method above.

		// Create an iterator to look through our uniform map and try to find the named uniform
		static map<string, int>::iterator it = uniformLocList.find(uniform);

		// Found it? Great - pass it back! Didn't find it? Alert user and halt.
		GLint uniformAddr;
		if ( it != uniformLocList.end() )
		{
			uniformAddr = uniformLocList[uniform];
		}
		else
		{
			uniformAddr = glGetUniformLocation( id, uniform.c_str() );
			if(uniformAddr == -1)
			{
				cout << "Could not find uniform in shader program: " << uniform << " - location returned -1!" << endl;
				exit(-1);
			}
			else
			{
				uniformLocList[uniform] = uniformAddr;
			}
		}
		return uniformAddr;
	}
};

#endif // SHADER_PROGRAM_HPP
