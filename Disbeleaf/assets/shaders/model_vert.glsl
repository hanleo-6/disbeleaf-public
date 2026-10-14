#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

out vec2 o_texcoord;
out vec3 o_normal;
out vec3 o_frag_pos;
	
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    o_normal = normal;
    o_texcoord = texcoord;
    o_frag_pos = vec3(model * vec4(position, 1.0f));
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
