@type vertex 
#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoords;

out vec2 oTexCoords;

void main() {
    gl_Position = vec4(pos, 0.0, 1.0);
    oTexCoords = texCoords;
}

@type fragment 
#version 330 core

out vec4 FragColor;
in vec2 oTexCoords;

uniform sampler2D u_Scene;
uniform int u_ShowGrey;

void main() {
    vec3 grey_vec = vec3(0.21, 0.71, 0.07);
    vec3 color = texture(u_Scene, oTexCoords).rgb;
    FragColor = (u_ShowGrey == 1) ? vec4(vec3(dot(grey_vec, color)), 1.0) : vec4(color, 1.0);
}