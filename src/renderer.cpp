#include "renderer.hpp"
#include <random>

void draw_splash(g::asset::store& assets, const ld50::state& state)
{

}

void draw_game(g::asset::store& assets, const ld50::state& state)
{

}

ld50::renderer::renderer(g::asset::store& a) : assets(a)
{
	
	
	{ // create sky sphere geometry
		auto sphere = [](const vec<3>& p) -> float { return 100 - p.magnitude(); };
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

	{ // create noise textures
		std::default_random_engine generator;
		std::normal_distribution<float> distribution(128, 64);

		white_noise_rgb = g::gfx::texture_factory{ 512, 512 }
		.components(3)
		.type(GL_UNSIGNED_BYTE)
		.repeating()
		.pixelated()
		.fill([&](int x, int y, int z, unsigned char* pixel) {
			unsigned r = (unsigned)distribution(generator);
			pixel[0] = 0xff & (r >> 16);
			pixel[1] = 0xff & (r >> 8);
			pixel[2] = 0xff & (r >> 0);
		})
		.create();
	}
}

void ld50::renderer::render(ld50::state& state)
{
	glClearColor(0.5, 0.5, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	state.my.camera.aspect_ratio(g::gfx::aspect());
	auto rotation_only = state.my.camera.view();
	rotation_only[3] = { 0, 0, 0, 1 };



	{ // draw sky sphere
		glDepthMask(GL_FALSE);
		sky_sphere.using_shader(assets.shader("position_normal.vs+celestial_sphere.fs"))
			["u_view"].mat4(rotation_only)
			["u_proj"].mat4(state.my.camera.projection())
			["u_model"].mat4(mat<4, 4>::I())
			["u_noise"].texture(white_noise_rgb)
			.draw<GL_TRIANGLES>();
		glDepthMask(GL_TRUE);
	}



	// draw appropriately depending on state
	switch(state.current)
	{
		case game_state::splash: draw_splash(assets, state); break;
		case game_state::game: draw_game(assets, state); break;
	}
}
