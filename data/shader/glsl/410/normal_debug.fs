in vec3 v_normal;

out vec4 color;

void main(void)
{
	color = vec4((v_normal.xyz + 1.0) / 2.0, 1.0);
}
