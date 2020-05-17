#version 450 core

uniform mat4 pr_matrix;

uniform mat4 v_matrix;
uniform mat4 m_matrix;


layout(location = 0) in vec3 position;
layout (location = 0) out vec3 worldPos;


void main()
{
 
    gl_Position = pr_matrix  * v_matrix * m_matrix * vec4(position.xyz, 1.0);
	worldPos =  (m_matrix *vec4(position.xyz, 1.0)).xyz;
}



















