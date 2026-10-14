#version 330 core

in vec2 o_texcoord;
in vec3 o_normal;
in vec3 o_frag_pos;

out vec4 fragColour;

uniform sampler2D tex_sampler;

uniform vec3 pointlight_pos1;
// ...

struct Light {
	vec3 direction;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	vec3 colour;
};

// Material material {
// 	sampler2D diffuse;
// 	sampler2D specular;
// 	float shininess;
// };

uniform Light sunlight;

uniform vec3 view_pos;

void main()
{
	// blur kernel
	float kernel[9] = float[] (
		1, 2, 1,
		2, 4, 2,
		1, 2, 1
	); // values add to 16

	// Lighting:
	vec3 norm = normalize(o_normal);

	// Sunlight
	// ambient
	vec3 ambient = sunlight.ambient * sunlight.colour;
	// diffuse
	float diffuse_effect = max(dot(norm, sunlight.direction), 0.0);
	vec3 diffuse = sunlight.diffuse * diffuse_effect * sunlight.colour;
	// specular
	vec3 view_dir = normalize(view_pos - o_frag_pos);
	vec3 reflect_dir = reflect(-sunlight.direction, norm);
	float specular_effect = pow(max(dot(view_dir, reflect_dir), 0.0), 64);
	vec3 specular = sunlight.specular * specular_effect * sunlight.colour;

	// Pointlight 1
	// vec3 light_dir = normalize(pointlight_pos1 - o_frag_pos); // calculate lighting direction
	// // ambient
	// // float ambient_strength = 0.5;
	// vec3 ambient = (ambient_strength) * light_colour;
	// // diffuse 
	// float diffuse_effect = max(dot(norm, light_dir), 0.0);
	// vec3 diffuse = diffuse_effect * light_colour;
	// specular
	// float specular_strength = 0.5;
	// vec3 view_dir = normalize(view_pos - o_frag_pos);
	// vec3 reflect_dir = reflect(-light_dir, norm);
	// float specular_effect = pow(max(dot(view_dir, reflect_dir), 0.0), 64);
	// vec3 specular = specular_strength * specular_effect * light_colour;

	fragColour = texture(tex_sampler, o_texcoord);
	fragColour = fragColour * vec4(ambient + diffuse + specular, 1.0);
}
