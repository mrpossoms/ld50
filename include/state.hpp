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
	float e; //< eccentricity
	float a; //< semi-major axis
	float i; //< inclination
	float Ω; //< longitude of the ascending node
	float ω; //< argument of periapsis
	float ν; //< true anomoly

	inline mat<3, 3> perifocal_to_geocentric()
	{
        const auto c_Ω = cos(Ω), s_Ω = sin(Ω);
        const auto c_ω = cos(ω), s_ω = sin(ω);
        const auto c_i = cos(i), s_i = sin(i);

        // transformation matrix between perifocal coords to geocentric
        return {
            { c_Ω * c_ω - s_Ω * s_ω * c_i, -c_Ω * s_ω - s_Ω * c_ω * c_i,  s_Ω * s_i, },
            { s_Ω * c_ω + c_Ω * s_ω * c_i, -s_Ω * s_ω + c_Ω * c_ω * c_i, -c_Ω * s_i, },
            { s_ω * s_i,                   c_ω * s_i,                     c_i      , },
        };// .transpose();
	}

	inline vec<3> velocity()
	{
		auto µ = 1.f; // gravitational param
		const auto v_a = vec<3>{-sin(ν), e + cos(ν), 0.f} * sqrtf(µ / a);

		return perifocal_to_geocentric() * v_a;
	}

	inline vec<3> position()
	{
		auto µ = 1.f; // gravitational param
		const auto r_mag = a / (1.f + e * cos(ν));
		const auto r_p = vec<3>{r_mag * cos(ν), r_mag * sin(ν), 0};

		return perifocal_to_geocentric() * r_p;
	}
};

struct body : public dyn::particle
{
	float mass;
	float radius;
	std::vector<body> satellites;
	std::string model_name;

	vec<3> position_at(float t) const
	{
		return {};
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