layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;

out vec4 color;

void main(void)
{
	vec3 light_dir = -normalize(v_position); 
	color = vec4((v_normal.xyz + 1.0) / 2.0, 1.0);

	float ndl = min(1.0, dot(light_dir, v_normal) + 0.1);
	color.rgb *= ndl;
}
