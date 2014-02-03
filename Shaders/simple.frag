#version 140
 
///precision highp float; // needed only for version 1.30
 

out vec3 outColor;

in vec3 outNormal;
void main(void)
{
    outColor = outNormal;
}