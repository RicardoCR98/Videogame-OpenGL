#version 330 core
out vec4 FragColor;

in vec3 ourColor;

uniform float visible;

void main()
{
    vec4 transparentColor = vec4(ourColor, visible);
    FragColor = transparentColor;
}
