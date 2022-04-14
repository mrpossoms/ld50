#include "mechanics.hpp"

#include <g.h>
#include <random>
#include <variant>

vec<3> ld50::force_at_point(const ld50::state& state, const vec<3>& pos, float t)
{
	std::function<vec<3>(const ld50::body&, vec<3>)> net_acc = [&](const ld50::body& b, vec<3> pos) -> vec<3> 
	{
		auto G = 1;
		auto r = b.position_at(t) - pos;
		vec<3> acc = r.unit() * (G * b.mass) / std::max(0.001f, r.dot(r));

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

void ld50::handle_controls(ld50::state& state, std::unordered_map<std::string, g::game::object>& objects, float dt)
{
	static double xlast, ylast;
	float sensitivity = 0.5f;
	double xpos = 0, ypos = 0;
	auto mode = glfwGetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR);
	auto& player_ship = state.my_player();

	if (GLFW_CURSOR_DISABLED == mode)
	{
		glfwGetCursorPos(g::gfx::GLFW_WIN, &xpos, &ypos);
	}

	if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_ESCAPE))
	{
		glfwSetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (glfwGetMouseButton(g::gfx::GLFW_WIN, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		glfwSetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (glfwGetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		if (xlast != 0 || ylast != 0)
		{
			auto dx = xpos - xlast;
			auto dy = ypos - ylast;

			auto d_o = quat<>::from_axis_angle({ 0, 1, 0 }, -dx * dt * sensitivity) * quat<>::from_axis_angle({ 1, 0, 0 }, dy * dt * sensitivity);

			auto Q = glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_Q) == GLFW_PRESS;
			auto E = glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_E) == GLFW_PRESS;

			if (Q && E) 
			{
				player_ship.dyn_apply_global_force(player_ship.position, -player_ship.velocity); 
			}
			else if (Q) { d_o = quat<>::from_axis_angle({ 0, 0, -1 }, -dt) * d_o; }
			else if (E) { d_o = quat<>::from_axis_angle({ 0, 0, -1 }, dt) * d_o; }

			if (glfwGetMouseButton(g::gfx::GLFW_WIN, GLFW_MOUSE_BUTTON_2) != GLFW_PRESS)
			{
				player_ship.orientation = d_o * player_ship.orientation;
				state.my.camera.orientation = player_ship.orientation.inverse();
			}
			else
			{
				state.my.camera.orientation = state.my.camera.orientation * d_o.inverse();
			}

			if (glfwGetMouseButton(g::gfx::GLFW_WIN, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
			{
				auto f = ld50::force_at_point(state, player_ship.position, state.time);

				player_ship.dyn_apply_local_force({ 0, 0, 0 }, { 0, 0, -10 * std::get<float>(objects["player-ship.yaml"].traits()["gravity_thrust_mult"]) });
			}
		}
	}
	xlast = xpos; ylast = ypos;
}

const auto kStarDensity = 0.1;
const auto kStarRadius = 100;

static void populate(ld50::body& parent, unsigned bodies, std::default_random_engine& generator, float parent_radius)
{
	std::uniform_real_distribution<float> radius_dist(0.1, 0.5);
	std::uniform_real_distribution<float> stanard_dist(0, 1);
	std::uniform_real_distribution<float> sma_dist(parent_radius * 2, parent_radius * 4);
	float sma = parent_radius * 10;

	while (bodies > 0)
	{
		auto body_radius = radius_dist(generator) * parent_radius;
		ld50::body body(body_radius, 0.75 * M_PI * pow(body_radius, 3.f) * kStarDensity);

		sma += sma_dist(generator);
		//body.orbit.a = sma;
		//body.orbit.O = 2 * M_PI * stanard_dist(generator);
		// body.position = body.orbit.position(body.mass) + parent.position;
		// body.velocity = body.orbit.velocity(body.mass) + parent.velocity;
		
		// auto R = vec<3>{sma, 0, 0};
		// body.velocity = vec<3>::cross(R, {0, -1, 0}).unit() * sqrtf((parent.mass + body.mass) / sma) * 40;
		// body.position = parent.position + R;

		body.orbit_radius = sma;
		body.orbit_period = 0.1 * M_PI * sqrtf(pow(sma, 3) / parent.mass);
		body.orbit_true_anomoly = stanard_dist(generator);

		body.model_name = std::to_string(body_radius);

		parent.satellites.push_back(body);
		bodies -= 1;

		if (stanard_dist(generator) > 0.99)
		{
			auto body_satellites = (unsigned)ceil(radius_dist(generator) * bodies);
			bodies -= body_satellites;
			populate(parent.satellites[parent.satellites.size()-1], body_satellites, generator, body_radius);
		}
	}
}

void ld50::populate_solar_system(ld50::state& state, unsigned bodies, unsigned seed)
{
	unsigned spawned = 0;
	std::default_random_engine generator;

	ld50::body star(kStarRadius, 0.75 * M_PI * pow(kStarRadius, 3.f) * 0);
	state.bodies.push_back(star);

	populate(state.bodies[0], bodies - 1, generator, kStarRadius);
}

void ld50::update_body_velocities(ld50::state& state, float dt)
{
	state.for_each_body([&](ld50::body& b) -> bool {
		auto force = ld50::force_at_point(state, b.position, state.time);
		b.velocity += (force / b.mass) * dt;
		return true;
	});
}

void ld50::update_body_positions(ld50::state& state, ld50::body& body, const vec<3>& parent_pos)
{
	body.position = body.position_at(state.time) + parent_pos;

	for (auto& s : body.satellites)
	{
		update_body_positions(state, s, body.position);
	}
}