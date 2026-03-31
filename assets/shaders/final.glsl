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

// Source - https://stackoverflow.com/a/42179924
// Posted by masterxilo, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-31, License - CC BY-SA 4.0

// from http://www.java-gaming.org/index.php?topic=35123.0
vec4 cubic(float v){
    vec4 n = vec4(1.0, 2.0, 3.0, 4.0) - v;
    vec4 s = n * n * n;
    float x = s.x;
    float y = s.y - 4.0 * s.x;
    float z = s.z - 4.0 * s.y + 6.0 * s.x;
    float w = 6.0 - x - y - z;
    return vec4(x, y, z, w) * (1.0/6.0);
}

vec4 textureBicubic(sampler2D sampler, vec2 texCoords){
    vec2 texSize = vec2(textureSize(sampler, 0));
    vec2 invTexSize = 1.0 / texSize;

    texCoords = texCoords * texSize - 0.5;

    vec2 fxy = fract(texCoords);
    texCoords -= fxy;

    texCoords = clamp(texCoords, vec2(0.0), texSize - 1.0);

    vec4 xcubic = cubic(fxy.x);
    vec4 ycubic = cubic(fxy.y);

    vec4 c = texCoords.xxyy + vec2(-0.5, 1.5).xyxy;

    vec4 s = vec4(xcubic.xz + xcubic.yw, ycubic.xz + ycubic.yw);
    vec4 offset = c + vec4(xcubic.yw, ycubic.yw) / s;

    offset *= invTexSize.xxyy;

    vec4 sample0 = texture(sampler, offset.xz);
    vec4 sample1 = texture(sampler, offset.yz);
    vec4 sample2 = texture(sampler, offset.xw);
    vec4 sample3 = texture(sampler, offset.yw);

    float sx = s.x / (s.x + s.y);
    float sy = s.z / (s.z + s.w);

    return mix(
        mix(sample3, sample2, sx),
        mix(sample1, sample0, sx),
        sy
    );
}

void main() {
    // FragColor = textureBicubic(scene, oTexCoords);
    FragColor = texture(scene, oTexCoords);
}