#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 switchColor;

out vec2 TexCoords;
out vec3 color;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = aTexCoords;
    color = switchColor;
}
