#pragma once
#include <vector>
#include <g.dyn.h>
#include <g.camera.h>
#include <random>

using namespace xmath;
using namespace g;

namespace ld50
{

enum class game_state
{
	splash,
	game,
};

template<size_t N>
struct economic_state
{
	vec<N> supply;
	vec<N> demand;
	vec<N> prices;
	mat<N, N> inputs;

	economic_state() = default;

	economic_state(std::default_random_engine& gen)
	{
		supply = ld50::random_norm_vec<N>(gen).abs() * 2;
		demand = ld50::random_norm_vec<N>(gen).take_max({});
		prices = ld50::random_norm_vec<N>(gen).abs();

		{ // setup inputs
			// setup good input proportions the first three goods
			// are raw materials and have no inputs
			for (unsigned i = 3; i < N; i++)
			{
				inputs[i] = ld50::random_norm_vec<N>(gen).abs();
			}

			// create a tiered system of goods where goods at the bottom
			// row of the input matrix have the most other goods they depend
			// on for production
			for (int r = 0; r < N; r++)
			for (int c = 0; c < N; c++)
			{
				if ((r - 3) <= c) { inputs[r][c] = 0; }
			}
		}
	}
};


struct body : public dyn::particle
{
	float mass;
	float radius;
	std::vector<body> satellites;
	std::string name;
	economic_state<9> economy;

	float orbit_radius = 0;
	float orbit_period = 1;
	float orbit_true_anomoly = 0;

	body(float r, float m) : radius(r), mass(m) {}

	vec<3> position_at(float t) const 
	{
		auto& r = orbit_radius;
		auto& T = orbit_period;

		t = 0; // keep all bodies stationary
		auto v = (2 * M_PI * (t / T)) + orbit_true_anomoly;

		return vec<3>{cosf(v), 0, sinf(v)} * r;
	}
};

struct player : public dyn::rigid_body
{
	float fuel = 1000;
	float thrust_per_fuel = 1;

	float current_thrust = 0;

	std::string name = "player";

	player()
	{
		mass = 1;
	}

	player(const vec<3>& p)
	{
		position = p;
		mass = 1;
	}
};

struct state
{
	state() = default;

	game_state current = game_state::splash;

	game_state operator* () { return this->current; }

	std::default_random_engine generator;

	float time = 0;
	std::vector<body> bodies;
	std::vector<player> players;
	
	void for_each_body(std::function<bool(ld50::body&)> callback, ld50::body* current=nullptr) 
	{
		if (current)
		{
			if (!callback(*current)) { return; }

			for (auto& b : current->satellites)
			{
				for_each_body(callback, &b);
			}
		}
		else
		{
			for (auto& b : bodies)
			{
				for_each_body(callback, &b);
			}
		}
	}

	/**
	 * Convenience struct for local player
	 */
	struct {
		unsigned player_index = 0;
		game::camera_perspective camera;
		float zoom = 1;
	} my;

	struct {
		float radius = 0;
	} super_nova;

	inline player& my_player() { return players[my.player_index]; }
};

} // namespace game