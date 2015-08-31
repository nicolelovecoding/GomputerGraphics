#version 130

precision mediump float;

in vec4 in_position;
in vec4 in_color;
in vec4 in_normal;

uniform mat4 MVP;
uniform mat4 ModelMatrix;
uniform mat3 tr_in_Model;
uniform mat4 v_inverse;
uniform int light_botton;
vec3 lightDirection;
out vec4 vColor;
vec3 diffuse;
vec3 specular;
vec3 m_normal;
vec4 e_v;
vec3 s;

struct lightSource
{
  vec4 position;
  vec4 d_and_s;
};

lightSource light = lightSource(
    vec4(10.0, 10.0, 10.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0)
);
void main()
{
	lightDirection = normalize(vec3(light.position) - vec3(ModelMatrix * in_position));

	m_normal = normalize(tr_in_Model * normalize(in_normal.xyz));

	diffuse = vec3(light.d_and_s) * vec3(in_color) 
	* max(0.0,dot(lightDirection,m_normal));

	e_v = normalize(v_inverse * vec4(0.0,0.0,0.0,1.0) - ModelMatrix * in_position);
	s = normalize(lightDirection + e_v.xyz);

	specular = vec3(light.d_and_s) * vec3(in_color)
	* pow(max(0.0,dot(s,m_normal)),20);

    gl_Position = MVP * in_position;

	if (light_botton==1)
	    vColor = vec4(diffuse + specular,1.0);
	else
		vColor = in_color;
}
