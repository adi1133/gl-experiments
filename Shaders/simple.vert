#version 140

uniform mat4 tMat;
 
in vec3 inPos;
in vec3 inNormal;

smooth out vec3 outNormal;

void main(void)
{
	outNormal = inNormal;
    gl_Position = tMat * vec4(inPos, 1.0);
}