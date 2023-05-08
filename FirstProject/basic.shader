#vertex_shader
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modelview;

void main()
{
    mat4 mv = view * model;
    gl_Position = projection * mv * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}

#texture_fracment_shader
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
void main()
{
    FragColor = texture(ourTexture, TexCoord);
}

#basic_color_shader
#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec4 color;

void main()
{
    FragColor = color;
}