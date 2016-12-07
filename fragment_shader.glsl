#version 330 core

uniform vec4 vertex_color;
out vec4 color;
in vec2 texel;
uniform sampler2D texture_sampler;

void main() {
  color = texture(texture_sampler, texel);
}
