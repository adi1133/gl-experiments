#ifndef SHADER_HPP
#define SHADER_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::ifstream;

class Shader
{
private:
	GLuint id;         // The unique ID / handle for the shader

public:

	//prevent copy operators since the two instances get destroyed twice
	Shader() : id(0){}
	Shader(const Shader& src) = delete;
	Shader & operator=(const Shader&) = delete;

	GLuint getId() const
	{
		return id;
	}

	// Method to load the shader contents from a string
	bool loadFromString(const GLenum type, const string source)
	{
		if(id)
			return false;
		id = glCreateShader(type);
		// Associate the source with the shader id
		const char* sourcePtr = source.c_str();
		glShaderSource(id, 1, &sourcePtr, NULL);

		// Compile the shader
		glCompileShader(id);

		// Check the compilation status and report any errors
		GLint shaderStatus;
		glGetShaderiv(id, GL_COMPILE_STATUS, &shaderStatus);


		if(shaderStatus)
		{
			cout <<"shader compilation OK" << endl;
		}
		else
		{
			cout << "shader compilation failed"<< endl;
		}

		GLint length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		if(length)
		{
			GLchar strInfoLog[length];
			glGetShaderInfoLog(id, length, NULL, strInfoLog);
			cout << strInfoLog << endl;
		}
		return shaderStatus;
	}

	// Method to load the shader contents from a file
	bool loadFromFile(const GLenum type, const string filename)
	{
		ifstream file;
		file.open( filename.c_str() );

		if (!file.good() )
		{
			cout << "Failed to open file: " << filename << endl;
			return false;
		}

		// Create a string stream
		stringstream stream;

		// Dump the contents of the file into it
		stream << file.rdbuf();

		// Close the file
		file.close();

		bool status = loadFromString(type, stream.str());
		if(!status)
		{
			cout << "Error in file " << filename << endl;
		}
		return status;

	}

	virtual ~Shader()
	{
		if(id)
			glDeleteShader(id);
	}

};

#endif // SHADER_HPP
