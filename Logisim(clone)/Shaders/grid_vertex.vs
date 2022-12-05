#version 330 core
layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 translation;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 gridColor;

out vec3 color;

void main()
{
    mat4 model;
    model[0] = vec4(1.0, 0.0, 0.0, 0.0);
    model[1] = vec4(0.0, 1.0, 0.0, 0.0);
    model[2] = vec4(0.0, 0.0, 1.0, 0.0);
    model[3] = vec4(translation.x, translation.y, 0.0, 1.0);
    
    gl_Position = projection * view * model * vec4(vPos, 0.0, 1.0);
    color = gridColor;
}
