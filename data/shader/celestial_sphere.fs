layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;

out vec4 color;

uniform sampler2D u_noise;

void main(void)
{
	vec3 n = normalize(v_normal);

	float w_x = abs(dot(v_normal, vec3(1, 0, 0)));
	float w_y = abs(dot(v_normal, vec3(0, 1, 0)));
	float w_z = abs(dot(v_normal, vec3(0, 0, 1)));

	float sum = w_x + w_y + w_z;
	w_x /= sum;
	w_y /= sum;
	w_z /= sum;

	vec3 uvw = v_position;
	float noise = texture(u_noise, uvw.yz * 0.4).r * w_x;
	noise += texture(u_noise, uvw.xz * 0.4).g * w_y;
	noise += texture(u_noise, uvw.xy * 0.4).b * w_z;

	float p = dot(vec3(n.x, 0, n.z), n);
	vec3 textel_color = mix(vec3(0.0, 0.0, 0.0), vec3(0.25, 0.25, 0.25), pow(p, 32));
	color = vec4(textel_color, 1.0);

	if (noise * p < 0.6)
	{
		noise = 0;
	}

	color.rgb += vec3(noise);
}
