#pragma once
#include <vector>
#include <g.dyn.h>
#include <g.camera.h>

using namespace xmath;
using namespace g;

namespace ld50
{

enum class game_state
{
	splash,
	game,
};

struct kepler
{
	float e = 0; //< eccentricity
	float a = 0; //< semi-major axis
	float i = 0; //< inclination
	float O = 0; //< longitude of the ascending node
	float o = 0; //< argument of periapsis
	float ν = 0; //< true anomoly

	float E = M_PI;
	float T = 0;

	inline mat<3, 3> perifocal_to_geocentric()
	{
        const auto c_O = cos(O), s_O = sin(O);
        const auto c_o = cos(o), s_o = sin(o);
        const auto c_i = cos(i), s_i = sin(i);

        // transformation matrix between perifocal coords to geocentric
        return {
            { c_O * c_o - s_O * s_o * c_i, -c_O * s_o - s_O * c_o * c_i,  s_O * s_i, },
            { s_O * c_o + c_O * s_o * c_i, -s_O * s_o + c_O * c_o * c_i, -c_O * s_i, },
            { s_o * s_i,                   c_o * s_i,                     c_i      , },
        };// .transpose();
	}

	// inline vec<3> velocity(float mass)
	// {
	// 	auto mu = mass * 1.f; // gravitational param
	// 	auto n = sqrtf(mu/pow(a,3));
	// 	auto M = n * t;

	// 	// M = E - e sin(E)
	// 	auto k_fn = [this](float E) -> float { return E - e * sin(E); };
	// 	for (; fabsf(k_fn(E) - M) > 0.0001;)
	// 	{
	// 		const auto h = 0.5f;

	// 		//auto dk = (k_fn(E + h) - k_fn(E)) / h;
	// 		auto r = k_fn(E) - M;
	// 		E -= r * h;

	// 		// std::cerr << r << " " << E << std::endl;
	// 	}

	// 	auto p = vec<3>{ -sinf(E), 0, sqrtf(1.f - e * e) * cosf(E) } * sqrtf(mu * a)/;
	//  	return perifocal_to_geocentric() * p;
	//  }

	//  inline vec<3> position(float mass)
	//  {
	//  	auto µ = mass * 1.f; // gravitational param
	//  	const auto r_mag = a / (1.f + e * cos(ν));
	//  	const auto r_p = vec<3>{r_mag * cos(ν), r_mag * sin(ν), 0};

	//  	return perifocal_to_geocentric() * r_p;
	//  }
};

struct body : public dyn::particle
{
	float mass;
	float radius;
	std::vector<body> satellites;
	std::string model_name;

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