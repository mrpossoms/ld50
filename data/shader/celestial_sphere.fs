layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;

out vec4 color;

uniform sampler2D u_noise;

void main(void)
{
	vec3 n = normalize(v_normal);

	vec3 g = v_position;
	float w_x = abs(dot(v_normal, vec3(1, 0, 0)));
	float w_y = abs(dot(v_normal, vec3(0, 1, 0)));
	float w_z = abs(dot(v_normal, vec3(0, 0, 1)));

	float sum = w_x + w_y + w_z;
	w_x /= sum;
	w_y /= sum;
	w_z /= sum;

	vec3 uvw = v_position;
	float noise = texture(u_noise, uvw.yz).r * w_x;
	noise += texture(u_noise, uvw.xz).g * w_y;
	noise += texture(u_noise, uvw.xy).b * w_z;

	float w = (dot(v_normal, vec3(0, 0, -1)) + 1.0) * 0.5;
	vec3 color_shift_ecliptic = mix(vec3(0.05, 0.05, 0.15), vec3(0.15, 0.05, 0.05), w);
	float p = dot(vec3(n.x, 0, n.z), n);
	vec3 textel_color = mix(vec3(0.0, 0.0, 0.0), color_shift_ecliptic, pow(p, 32)) + mix(vec3(0.0, 0.0, 0.0), color_shift_ecliptic * 0.5, pow(p, 1));
	color = vec4(textel_color, 1.0);

	if ((noise * p) < 0.9)
	{
		noise = 0;
	}

	color.rgb += vec3(noise);
}
