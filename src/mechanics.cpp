#include "mechanics.hpp"
#include <random>

vec<3> ld50::acceleration_at_point(const ld50::state& state, const vec<3>& pos, float t)
{
	std::function<vec<3>(const ld50::body&, vec<3>)> net_acc = [&](const ld50::body& b, vec<3> pos) -> vec<3> 
	{
		auto G = 1;
		auto r = b.position - pos;
		vec<3> acc = r * G * b.mass / std::max(0.001f, r.dot(r));

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

const auto kStarDensity = 0.0001;
const auto kStarRadius = 100;

static void populate(ld50::body& parent, unsigned bodies, std::default_random_engine& generator, float parent_radius)
{
	std::uniform_real_distribution<float> radius_dist(0.01, 0.5);
	std::uniform_real_distribution<float> stanard_dist(0, 1);
	std::uniform_real_distribution<float> sma_dist(parent_radius * 10, parent_radius * 30);
	float sma = 0;

	while (bodies > 0)
	{
		auto r = radius_dist(generator) * parent_radius;
		ld50::body body(r, pow(r, 3.f) * kStarDensity);

		sma += sma_dist(generator);
		body.orbit.a = sma;
		body.orbit.O = 2 * M_PI * stanard_dist(generator);
		body.model_name = std::to_string(r);

		body.position = body.orbit.position(body.mass) + parent.position;
		body.velocity = body.orbit.velocity(body.mass) + parent.velocity;

		parent.satellites.push_back(body);
		bodies -= 1;

		if (stanard_dist(generator) > 0.25)
		{
			auto body_satellites = (unsigned)ceil(radius_dist(generator) * bodies);
			bodies -= body_satellites;
			populate(parent.satellites[parent.satellites.size()-1], body_satellites, generator, r);
		}
	}
}

void ld50::populate_solar_system(ld50::state& state, unsigned bodies, unsigned seed)
{
	unsigned spawned = 0;
	std::default_random_engine generator;

	ld50::body star(kStarRadius, pow(kStarRadius, 3.f) * kStarDensity);
	state.bodies.push_back(star);

	populate(state.bodies[0], bodies - 1, generator, kStarRadius);
}

void ld50::update_body_velocities(ld50::state& state)
{
	state.for_each_body([&](ld50::body& b) -> bool {
		auto acc = ld50::acceleration_at_point(state, b.position, state.time);
		b.velocity += acc;
		return true;
	});
}

void ld50::update_body_positions(ld50::state& state, float dt)
{
	state.for_each_body([&](ld50::body& b) {
		b.position += b.velocity * dt;
		return true;
	});
}