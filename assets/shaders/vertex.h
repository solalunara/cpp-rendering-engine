// This file is a copy of a file that originated in 2022 by the same author
// as part of a project that received periodic updates through 2024
// the file in that project was a derivative of a file originally written by Joey de Vries made avaliable via learnopengl.com
// where much of the original logic for interfacing with opengl was written

const char *g_pszVertexShaderSource = "\
#version 330 \n\
layout (location = 0) in vec3 pos; \
layout (location = 1) in vec2 InTexCoord; \
out vec2 TexCoord; \
uniform mat4 Transform; \
uniform mat4 CameraTransform; \
uniform mat4 Perspective; \
uniform float TextColorX; \
uniform float TextColorY; \
uniform float TextColorZ; \
out vec3 TextColor; \
uniform int Text; \
flat out int TextF; \
uniform int UI; \
void main() \
{ \
    if ( UI != 0 ) \
        gl_Position = Transform * vec4( pos, 1.0 ); \
    else \
        gl_Position = Perspective * CameraTransform * Transform * vec4( pos, 1.0 ); \
    TexCoord = InTexCoord; \
    TextColor = vec3( TextColorX, TextColorY, TextColorZ ); \
    TextF = Text; \
} \
";
