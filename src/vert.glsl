// GLSL Vertex Shader
// Pass-through position, pass-through per-primitive (uniform) color
// Jeremy Carter, Spring 2012

#version 150

in vec4 vColor;

in vec4 vPosition;

out vec4 color;

void main()
{
    gl_Position = vPosition;
    color = vColor;
	//color = new vec4(1,1,0,.5);
}
