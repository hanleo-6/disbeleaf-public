#version 330 core

out vec4 fragColour;
in vec3 o_texcoord;

uniform samplerCube skybox_sampler;

void main()
{   
	fragColour = texture(skybox_sampler, o_texcoord);
}
