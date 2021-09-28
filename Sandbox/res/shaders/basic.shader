#type vertex
#version 330 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;

out vec4 v_color;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_position;
    v_color = a_color;
}

#type fragment
#version 330 core

in vec4 v_color;

layout(location = 0) out vec4 color;

void main()
{
    color = v_color;
}