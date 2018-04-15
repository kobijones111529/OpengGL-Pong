#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform mat4 u_Model;

out vec4 v_Position;
out vec4 v_Color;

void main() {
	v_Position = u_Model * position;
	v_Color = color;
	gl_Position = u_Model * position;
}