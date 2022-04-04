#pragma once
#include "state.hpp"
#include <g.assets.h>

namespace ld50
{

struct renderer
{
	renderer(g::asset::store& assets);

	void render(ld50::state& state);

private:
	g::gfx::texture white_noise_rgb;
	g::gfx::mesh<g::gfx::vertex::pos_norm> sky_sphere;
	g::asset::store& assets;
};

} // namespace ld50