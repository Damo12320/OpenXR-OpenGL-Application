#version 330 core
out vec4 FragColor;
  
in vec3 normal;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
    //FragColor = vec4(normal, 1);
    //FragColor = vec4(1);
}