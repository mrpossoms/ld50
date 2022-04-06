in vec3 a_position;
in vec2 a_uv;
in vec3 a_normal;

layout(location = 0) out vec3 v_position;
layout(location = 1) out vec2 v_uv;
layout(location = 2) out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main(void)
{
	vec4 v_world_pos = u_model * vec4(a_position, 1.0);
	gl_Position = u_proj * u_view * v_world_pos;

	v_position = v_world_pos.xyz;
	v_uv = a_uv;

	mat3 model_rot = mat3(normalize(u_model[0].xyz), normalize(u_model[1].xyz), normalize(u_model[2].xyz));
	v_normal = normalize(model_rot * a_normal);
}
