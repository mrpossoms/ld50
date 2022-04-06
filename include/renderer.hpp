#pragma once
#include "state.hpp"
#include <g.assets.h>
#include <g.game.object.h>

#include <unordered_map>

namespace ld50
{

struct renderer
{
	renderer(g::asset::store& assets, std::unordered_map<std::string, g::game::object>& object_map);

	void render(ld50::state& state);

private:
	g::gfx::texture white_noise_rgb;
	g::gfx::mesh<g::gfx::vertex::pos_norm> sky_sphere;
	g::asset::store& assets;
	std::unordered_map<std::string, g::game::object>& object_map;
};

} // namespace ld50