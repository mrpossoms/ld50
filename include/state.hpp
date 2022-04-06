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
};

struct player : public dyn::rigid_body
{
	float fuel;
	float thrust_per_fuel;
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

	} my;

	struct {
		float radius = 0;
	} super_nova;
};

} // namespace game