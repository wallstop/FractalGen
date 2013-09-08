// GLSL Fragment Shader
// Pass-through color
// Jeremy Carter, Spring 2012

#version 150

in vec4 color;

void main()
{
    gl_FragColor = color;
}
