#pragma once
#include <xmath.h>

namespace ld50
{

template<size_t N>
vec<N> random_norm_vec(std::default_random_engine& generator, float mean=0, float stddev=1)
{
	std::normal_distribution<float> n(mean, stddev);
	vec<N> out;

	for (unsigned i = 0; i < N; i++) { out[i] = n(generator); }

	return out;
}

static
vec<3> random_vec(std::default_random_engine& generator, const vec<3>& min = { -1, -1, -1 }, const vec<3> max = { 1, 1, 1 })
{
	std::uniform_real_distribution<float> x(min[0], max[0]);
	std::uniform_real_distribution<float> y(min[1], max[1]);
	std::uniform_real_distribution<float> z(min[2], max[2]);

	return { x(generator), y(generator), z(generator) };
}

} // namespace ld50