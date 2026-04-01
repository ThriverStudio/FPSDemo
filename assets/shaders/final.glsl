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

uniform sampler2D scene;

void main() {
    // TODO: Add post processing effects here later
    FragColor = texture(scene, oTexCoords);
}