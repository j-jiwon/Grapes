#version 330 core
in vec4 vertexColor;
in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D tex;
uniform sampler2D tex2;  // add another texture

void main() {
    // blend textures with a weight of 0.8, 0.2
    fragColor = texture(tex, texCoord) * 0.8 + texture(tex2, texCoord) * 0.2;
}
