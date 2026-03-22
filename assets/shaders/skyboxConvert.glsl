@type vertex 
#version 330 core

layout (location = 0) in vec3 pos;

out vec3 oWorldPos;

uniform mat4 proj;
uniform mat4 view;

void main() {
    oWorldPos = pos;
    gl_Position = proj * view * vec4(pos, 1.0);
}

@type fragment
#version 330 core

out vec4 FragColor;

in vec3 oWorldPos;

uniform sampler2D tex;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 sampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec3 dir = normalize(oWorldPos);
    vec2 uv = sampleSphericalMap(dir);
    vec3 color = texture(tex, uv).rgb;

    FragColor = vec4(color, 1.0);
}
