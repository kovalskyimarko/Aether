#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 objectColor;
uniform bool useColor;
uniform bool useTexture;
uniform sampler2D texture1;

void main()
{
    vec4 baseColor = vec4(1.0);

    if (useTexture)
    {
        baseColor = texture(texture1, TexCoord);
    }

    if (useColor)
    {
        baseColor *= vec4(objectColor, 1.0);
    }

    FragColor = baseColor;
}

