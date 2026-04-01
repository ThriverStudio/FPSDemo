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
out vec3 oFragPos;

void main() {
	vec4 worldPos = model * vec4(pos, 1.0);
	gl_Position = proj * view * worldPos;
	
	oTexCoord = texCoord;
	oNormal = normal;
	oFragPos = worldPos.xyz;
}

@type fragment
#version 330 core

out vec4 FragColor;

in vec2 oTexCoord;
in vec3 oNormal;
in vec3 oFragPos;

uniform vec3 cameraPos;
uniform float maxMipLevel;

uniform sampler2D lightMap;

void main() {
	FragColor = texture(lightMap, oTexCoord);
}