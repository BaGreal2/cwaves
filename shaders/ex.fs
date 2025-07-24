#version 330

uniform float uTime;

in vec2 fragTexCoord;
out vec4 fragColor;

void main()
{
  fragColor = vec4(fragTexCoord.x, fragTexCoord.y, 0.5, 1.0);
}
