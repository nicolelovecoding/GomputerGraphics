#version 130
precision mediump float;

in vec4 in_position;
in vec4 in_color;
out vec4 vColor;

void main()
{
 
    gl_Position = in_position;
	vColor = in_color;

}
