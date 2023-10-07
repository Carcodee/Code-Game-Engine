#version 330 core
out vec4 FragColor;

uniform vec3 UniqueColor;  // Set this unique color for each object

void main()
{
    FragColor = vec4(UniqueColor, 1.0);
}