#include "mechanics.hpp"

vec<3> ld50::acceleration_at_point(const ld50::state& state, const vec<3>& pos, float t)
{
	std::function<vec<3>(const vec<3>&, const ld50::body&, vec<3>)> net_acc = [&](const vec<3>& parent_pos, const ld50::body& b, vec<3> pos) -> vec<3> 
	{
		auto G = 1;
		auto p = parent_pos + b.position_at(t);
		auto r = p - pos;
		vec<3> a = r * G * b.mass / r.dot(r);

		for (auto& b : b.satellites)
		{
			a += net_acc(p, b, pos);
		}

		return a;
	};

	vec<3> a = {};

	for (auto& b : state.bodies)
	{
		a += net_acc(b.position_at(t), b, pos);
	}

	return a;
}
