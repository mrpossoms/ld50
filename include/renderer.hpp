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

	void draw_splash(ld50::state& state);
	void draw_game(ld50::state& state);

	void render_bodies(ld50::body& b, g::game::camera& cam);

	g::gfx::texture white_noise_rgb;
	g::gfx::mesh<g::gfx::vertex::pos_norm> sky_sphere;
	std::unordered_map<std::string, g::gfx::mesh<g::gfx::vertex::pos_norm>> planet_meshes;
	g::asset::store& assets;
	std::unordered_map<std::string, g::game::object>& object_map;

	std::unordered_map<std::string, g::gfx::mesh<g::gfx::vertex::pos_norm_color>> voxel_meshes;
};

} // namespace ld50
