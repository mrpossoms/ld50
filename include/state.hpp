#pragma once
#include <vector>
#include <g.dyn.h>

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
};

struct player : public dyn::rigid_body
{
	float fuel;
	float thrust_per_fuel;
};

struct state
{
	game_state state;

	std::vector<body> bodies;
	std::vector<player> players;


};

} // namespace game