// Vertex shader by thuong.nv 2023.05.28

#version 330 core

// VERTEX SHADER
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 tex_coord;

uniform mat4 viewmat;
uniform mat4 projmat;
uniform mat4 modelmat;

void main()
{
    //alway projection matrix * view matrix . if not error
    gl_Position = projmat * viewmat * vec4(aPos, 1.0);
    tex_coord = aTexCoord;
}