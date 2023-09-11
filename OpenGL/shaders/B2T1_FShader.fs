#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D textura;
uniform float visible;

void main()
{
	FragColor = mix(texture(textura, TexCoord), vec4(ourColor, 1.0f), visible);
}