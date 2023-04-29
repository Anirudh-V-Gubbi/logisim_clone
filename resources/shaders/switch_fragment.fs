#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;
in vec3 color;

uniform sampler2D switchTexture;
uniform sampler2D colorMapTexture;

void main()
{
    FragColor = texture(switchTexture, TexCoords) + texture(colorMapTexture, TexCoords) * vec4(color, 1.0f);
}
