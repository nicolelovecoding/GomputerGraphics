#version 130
precision mediump float;

in vec4 in_position;
in vec4 in_color;
out vec4 vColor;
uniform vec4 Transform;
uniform mat4 Scale = mat4(1,0,0,0,
                          0,1,0,0,
						  0,0,1,0,
						  0,0,0,1);

uniform mat4 Rotate = mat4(1,0,0,0,
                           0,1,0,0,
						   0,0,1,0,
						   0,0,0,1);

void main()
{   
    gl_Position = Transform + Rotate * Scale * in_position; 
	vColor = in_color;

}
