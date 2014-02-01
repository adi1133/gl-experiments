#version 140

uniform mat4 tMat;
 
in  vec3 inPos;

void main(void)
{
        gl_Position = tMat * vec4(inPos, 1.0);
}