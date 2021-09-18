#vertex shader
#version 330 core

layout(location = 0) in vec4 a_position;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_position;
}

#fragment shader
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(0.3, 0.2, 0.9, 1.0);
}