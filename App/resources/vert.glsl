#version 450 core

layout(location = 0) in vec2 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transformation;

void main()
{
	gl_Position = u_ViewProjection * u_Transformation * vec4(a_Position, 0.0f, 1.0f);
}