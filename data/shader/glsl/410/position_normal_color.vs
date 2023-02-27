in vec3 a_position;
in vec3 a_normal;
in vec4 a_color;

out vec3 v_position;
out vec4 v_view_position;
out vec3 v_normal;
out vec4 v_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main(void)
{
	vec4 v_world_pos = u_model * vec4(a_position, 1.0);
	v_view_position = u_view * v_world_pos;
	gl_Position = u_proj * v_view_position;

	v_position = v_world_pos.xyz;

	mat3 model_rot = mat3(normalize(u_model[0].xyz), normalize(u_model[1].xyz), normalize(u_model[2].xyz));
	v_normal = normalize(model_rot * a_normal);
	v_color = a_color;
}
