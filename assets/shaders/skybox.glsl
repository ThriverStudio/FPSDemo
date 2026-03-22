@type vertex 
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 oTexCoords;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    oTexCoords = aPos;

    mat4 rotView = mat4(mat3(view));
    vec4 pos = proj * rotView * vec4(aPos, 1.0);

    gl_Position = pos.xyww;
}


@type fragment
#version 330 core

out vec4 FragColor;

in vec3 oTexCoords;

uniform samplerCube tex;

void main() {
    FragColor = texture(tex, oTexCoords);
}
