#include "renderer.hpp"

void draw_splash(g::asset::store& assets, const ld50::state& state)
{

}

void draw_game(g::asset::store& assets, const ld50::state& state)
{

}

ld50::renderer::renderer(g::asset::store& a) : assets(a)
{
	auto sphere = [](const vec<3>& p) -> float { return 1 - p.magnitude(); };
	auto gen = [](const g::game::sdf& sdf, const vec<3>& pos) -> g::gfx::vertex::pos_norm
	{
		return { pos, -pos.unit() };
	};
	vec<3> corners[] = {
		{ -100,-100,-100 },
		{  100, 100, 100 },
	};

	sky_sphere = g::gfx::mesh_factory::from_sdf<g::gfx::vertex::pos_norm>(sphere, gen, corners);
}

void ld50::renderer::render(ld50::state& state)
{
	glClearColor(0.5, 0.5, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sky_sphere.using_shader(assets.shader("position_normal.vs+normal_debug.fs"))
	.set_camera(state.my.camera)
	["u_model"].mat4(mat<4, 4>::I())
	.draw<GL_TRIANGLES>();

	// draw appropriately depending on state
	switch(state.current)
	{
		case game_state::splash: draw_splash(assets, state); break;
		case game_state::game: draw_game(assets, state); break;
	}
}
