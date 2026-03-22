@type vertex
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 proj;
uniform mat4 model;
uniform mat4 view;

out vec2 oTexCoord;
out vec3 oNormal;

void main() {
	gl_Position = proj * view * model * vec4(pos, 1.0);
	oTexCoord = texCoord;
	oNormal = normal;
}

@type fragment
#version 330 core

out vec4 FragColor;

in vec2 oTexCoord;
in vec3 oNormal;

uniform sampler2D tex;

void main() {
	vec3 lightDir = vec3(-1.0);
	float diffuseFactor = max(dot(oNormal, -lightDir), 0.0);

	FragColor = texture(tex, oTexCoord) * diffuseFactor;
}