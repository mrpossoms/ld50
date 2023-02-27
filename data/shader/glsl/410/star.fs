layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;

out vec4 color;

void main(void)
{
	vec3 light_dir = -normalize(v_position); 
	color = vec4(1.0, 1.0, 1.0, 1.0);
}
