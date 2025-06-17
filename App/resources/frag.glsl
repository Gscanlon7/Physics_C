#version 450 core

layout(location = 0) out vec4 o_Color;

uniform vec3 u_Color = vec3(1.0f, 1.0f, 1.0f);

void main()
{
	o_Color = vec4(u_Color, 1.0f);
}