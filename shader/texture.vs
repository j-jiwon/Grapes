# version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 TexCoord;

uniform mat4 transform;

out vec4 vertexColor;
out vec2 texCoord;

void main() {
    gl_Position = transform * vec4(Pos, 1.0);
    vertexColor = vec4(Color, 1.0);
    texCoord = TexCoord;
}
