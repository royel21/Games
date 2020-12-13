#ifndef _TEXTLAYERSHADER_H
#define _TEXTLAYERSHADER_H

const char *vertTextLayer = R"END(#version 330 core
layout (location = 0) in vec4 vertex;
out vec2 TexCoords;

uniform mat4 projMatrix;

void main()
{
    gl_Position = projMatrix * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
})END";

const char *fragTextLayer = R"END(#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = textColor * sampled;
})END";

#endif
