#vertex shader
#version 330 core

layout(location = 0) in vec4 a_position;

out vec4 v_position;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_position;
    v_position = u_MVP * a_position;
}

#fragment shader
#version 330 core

in vec4 v_position;

layout(location = 0) out vec4 color;

void main()
{
    color = 0.5 + v_position * 0.5;
}