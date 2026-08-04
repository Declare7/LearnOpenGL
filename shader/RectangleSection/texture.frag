#version 330 core
out vec4 FragColor;

in vec4 vertColor;
in vec2 texCoord;

uniform sampler2D uniTexture;
void main()
{
    // FragColor = vertColor;
    // FragColor = texture(uniTexture, texCoord);
    FragColor = texture(uniTexture, texCoord) * vertColor;
}
