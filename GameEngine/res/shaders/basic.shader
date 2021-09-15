#vertex shader
#version 330 core

layout(location = 0) in vec3 a_position;

out vec3 v_position;

void main()
{
    gl_Position = vec4(a_position, 1.0);
    v_position = a_position;
}

#fragment shader
#version 330 core

in vec3 v_position;

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(0.5 + v_position * 0.5, 1.0);
}