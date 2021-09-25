#vertex shader
#version 330 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_texCoord;

out vec2 v_texCoord;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * a_position;
    v_texCoord = a_texCoord;
}

#fragment shader
#version 330 core

in vec2 v_texCoord;

layout(location = 0) out vec4 color;

uniform sampler2D u_texture;

void main()
{
    color = texture(u_texture, v_texCoord);
}