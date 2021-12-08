#type vertex
#version 330 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in float a_texIndex;
layout(location = 4) in vec2 a_texOffset;
layout(location = 5) in vec2 a_texScaleFactor;

out vec4 v_color;
out vec2 v_texCoord;
out float v_texIndex;
out vec2 v_texOffset;
out vec2 v_texScaleFactor;

uniform mat4 u_viewProjectionMatrix;

void main()
{
    gl_Position = u_viewProjectionMatrix * a_position;
    v_color = a_color;
    v_texCoord = a_texCoord;
    v_texIndex = a_texIndex;
    v_texOffset = a_texOffset;
    v_texScaleFactor = a_texScaleFactor;
}

#type fragment
#version 330 core

in vec4 v_color;
in vec2 v_texCoord;
in float v_texIndex;
in vec2 v_texOffset;
in vec2 v_texScaleFactor;

layout(location = 0) out vec4 color;

uniform sampler2D u_textures[32];

void main()
{
    color = texture(u_textures[int(v_texIndex)], v_texOffset + v_texCoord * v_texScaleFactor) * v_color;
    //color = v_color;
}
