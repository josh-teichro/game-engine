#type vertex
#version 330 core

layout(location = 0) in vec4 a_position;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_position;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;

void main()
{
    color = u_color;
}