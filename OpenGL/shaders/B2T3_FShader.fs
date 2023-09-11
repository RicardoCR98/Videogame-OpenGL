#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
//uniform sampler2D texture2;
// factor de interpolación --> uniform float visible;

void main()
{
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), visible);
	FragColor = texture(texture1, TexCoord);
}

 
