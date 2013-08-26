#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D textureSampler;
uniform vec4 alphaColor;

void main() {
    color = texture2D(textureSampler, UV).rgba * alphaColor;
}
