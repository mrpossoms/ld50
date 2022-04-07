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

	void ld50::renderer::draw_splash(ld50::state& state);
	void ld50::renderer::draw_game(ld50::state& state);

	g::gfx::texture white_noise_rgb;
	g::gfx::mesh<g::gfx::vertex::pos_norm> sky_sphere;
	g::asset::store& assets;
	std::unordered_map<std::string, g::game::object>& object_map;
};

} // namespace ld50