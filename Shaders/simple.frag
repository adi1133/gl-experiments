#version 140
 
///precision highp float; // needed only for version 1.30
 

out vec3 outColor;

in vec3 outNormal;
in vec3 outNormal2;
void main(void)
{
    outColor = vec3(0.5, 0 ,1);
	outColor = outNormal + outNormal2;
}