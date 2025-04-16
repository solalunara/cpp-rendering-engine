// This file is a copy of a file that originated in 2022 by the same author
// as part of a project that received periodic updates through 2024
// the file in that project was a derivative of a file originally written by Joey de Vries made avaliable via learnopengl.com
// where much of the original logic for interfacing with opengl was written


const char *g_pszFragShaderSource = "\
#version 330 \n\
in vec2 TexCoord; \
in vec3 TextColor; \
out vec4 FragColor; \
uniform sampler2D TextureToRender; \
flat in int TextF; \
void main() \
{ \
    if ( TextF == 0 ) \
        FragColor = texture( TextureToRender, TexCoord ); \
    else \
        FragColor = vec4( TextColor, 1.0 ) * vec4( 1.0, 1.0, 1.0, texture( TextureToRender, TexCoord ).r ); \
    if ( FragColor.a < .1f ) discard; \
} \
";
