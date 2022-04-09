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
	float a = 1; //< semi-major axis
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

	inline vec<3> position_at(float mass, float t)
	{
		auto mu = mass * 1.f; // gravitational param
		auto n = sqrtf(mu/pow(a,3));
		auto M = n * t;

		// M = E - e sin(E)
		//
		auto k_fn = [this](float E) -> float { return E - e * sin(E); };
		for (; fabsf(k_fn(E) - M) > 0.001;)
		{
			const auto h = 0.01f;

			auto dk = (k_fn(E + h) - k_fn(E)) / h;
			auto r = k_fn(E) - M;
			E += dk * h;

			std::cerr << r << " " << E << std::endl;
		}

		auto ta_fn = [this](float T) -> float { return (1 - e) * powf(tanf(T/2), 2.f); };
		auto E_ta = (1 + e) * powf(tanf(E / 2), 2.f);
		for (; fabsf(ta_fn(T) - E_ta) > 0.001;)
		{
			const auto h = 0.5f;

			auto r = ta_fn(T) - E_ta;
			T -= r * h;
			//std::cerr << r << " " << E << std::endl;
		}
		//std::cerr << E << std::endl;

		auto r = a * (1 - e * cos(E));

		return quat<>::from_axis_angle({ 0, 1, 0 }, T).rotate({ 0, 0, r });
	};

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

	kepler orbit;

	vec<3> position_at(float t)
	{
		return orbit.position_at(mass, t);
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