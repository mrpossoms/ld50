#include "mechanics.hpp"

vec<3> ld50::acceleration_at_point(const ld50::state& state, const vec<3>& pos, float t)
{
	std::function<vec<3>(const ld50::body&, vec<3>)> net_acc = [&](const ld50::body& b, vec<3> pos) -> vec<3> 
	{
		auto G = 1;
		auto r = b.position - pos;
		vec<3> acc = r * G * b.mass / r.dot(r);

		for (auto& b : b.satellites)
		{
			acc += net_acc(b, pos);
		}

		return acc;
	};

	vec<3> a = {};

	for (auto& b : state.bodies)
	{
		a += net_acc(b, pos);
	}

	return a;
}


void ld50::update_bodies(ld50::state& state, ld50::body& b, const vec<3>& parent_pos)
{
	b.position = parent_pos + b.position_at(state.time);

	for (auto& s : b.satellites)
	{
		ld50::update_bodies(state, s, b.position);
	}
}