layout(location=0) in vec4 v_color;
layout(location=1) in vec4 v_view_position;

out vec4 color;

void main(void)
{
	color = v_color;
	color.a = 1000 / length(v_view_position);
}
