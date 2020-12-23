#pragma once

#include <string>

const char *vertexShader2 =
    R"END(#version 330
layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 vertexColor;

out vec2 fragmentPosition;
out vec2 fragmentUV;
out vec4 fragmentColor;

uniform mat4 camera;

void main() {
    //Set the x,y position on the screen
    gl_Position.xy = (camera * vec4(vertexPosition, 0, 1.0)).xy;

    gl_Position.z = 0.0;

	gl_Position.w = 1.0;

    fragmentPosition = vertexPosition;
    
    fragmentColor = vertexColor;
    
    fragmentUV = vertexUV;
})END";

const char *fragShader2 =
    R"END(#version 330 core
in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;
uniform int hasTexture = 0;
uniform sampler2D mySampler;

void main() {

    vec4 textureColor = texture2D(mySampler, fragmentUV);

	if(hasTexture == 1){
		color = fragmentColor;
	}else{
		color = fragmentColor * textureColor;
	}
})END";
