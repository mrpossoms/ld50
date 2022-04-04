#pragma once
#include "state.hpp"
#include <g.assets.h>

namespace ld50
{

struct renderer
{
	renderer(g::asset::store& assets);

	void render(const ld50::state& state);

private:
	g::asset::store& assets;
};

} // namespace ld50