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

struct body : public dyn::particle
{
	float mass;
	std::vector<body> satellites;
	std::string model_name;

	float e; //< eccentricity
	float a; //< semi-major axis
	float i; //< inclination
	float W; //< longitude of the ascending node
	float w; //< argument of periapsis

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