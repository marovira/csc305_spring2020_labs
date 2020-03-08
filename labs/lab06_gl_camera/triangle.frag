#version 450 core

in vec3 vertexColour;

out vec4 fragColour;

void main()
{
    fragColour = vec4(vertexColour, 1.0);
}
