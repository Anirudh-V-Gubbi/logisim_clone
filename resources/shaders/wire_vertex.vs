#version 330 core
layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 translation;
layout (location = 2) in float rotation;

uniform vec2 size;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 wcolor;

out vec3 color;

void main()
{
    mat4 model;
    model[0] = vec4(size.x * cos(rotation), size.x * -sin(rotation), 0.0, 0.0);
    model[1] = vec4(size.y * sin(rotation), size.y * cos(rotation), 0.0, 0.0);
    model[2] = vec4(0.0, 0.0, 1.0, 0.0);
    model[3] = vec4(translation.x, translation.y, 0.0, 1.0);
    
    gl_Position = projection * view * model * vec4(vPos, 0.0, 1.0);
    color = wcolor;
}
