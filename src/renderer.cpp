#include "renderer.hpp"
#include <random>

using mat4 = xmath::mat<4, 4>;

void ld50::renderer::draw_splash(ld50::state& state)
{

}

static void render_bodies(g::asset::store& assets, ld50::body& b, g::game::camera& cam)
{
	auto& planet_shader = assets.shader("pos_uv_norm.vs+body.fs");

	assets.geo(b.model_name).using_shader(planet_shader)
		.set_camera(cam)
		["u_model"].mat4(mat4::I())
		.draw<GL_TRIANGLES>();

	for (auto& sat : b.satellites)
	{
		render_bodies(assets, sat, cam);
	}
}

void ld50::renderer::draw_game(ld50::state& state)
{
	auto rotation_only = state.my.camera.view();
	rotation_only[3] = { 0, 0, 0, 1 };



	{ // draw sky sphere
		glDepthMask(GL_FALSE);
		sky_sphere.using_shader(assets.shader("position_normal.vs+celestial_sphere.fs"))
			["u_view"].mat4(rotation_only)
			["u_proj"].mat4(state.my.camera.projection())
			["u_model"].mat4(mat4::I())
			["u_noise"].texture(white_noise_rgb)
			.draw<GL_TRIANGLES>();
		glDepthMask(GL_TRUE);
	}

	{ // draw bodies
		for (auto& body : state.bodies)
		{
			render_bodies(assets, body, state.my.camera);
		}
	}

	{ // draw players
		auto& player_shader = assets.shader("pos_uv_norm.vs+body.fs");

		glDisable(GL_CULL_FACE);
		for (auto& player : state.players)
		{
			object_map["data/player-ship.yaml"].geometry("hull").using_shader(player_shader)
				.set_camera(state.my.camera)
				["u_model"].mat4(player.orientation.to_matrix() * mat<4, 4>::translation(player.position))
				.draw<GL_TRIANGLES>();

			glDisable(GL_DEPTH_TEST);
			g::gfx::debug::print{ &state.my.camera }.color({ 1, 0, 0, 1 }).ray(player.position, player.to_global(vec<3>{ 2, 0, 0 }));
			g::gfx::debug::print{ &state.my.camera }.color({ 0, 1, 0, 1 }).ray(player.position, player.to_global(vec<3>{ 0, 2, 0 }));
			g::gfx::debug::print{ &state.my.camera }.color({ 0, 0, 1, 1 }).ray(player.position, player.to_global(vec<3>{ 0, 0, 2 }));
			glEnable(GL_DEPTH_TEST);
		}
		glEnable(GL_CULL_FACE);
	}

	g::gfx::debug::print{ &state.my.camera }.color({ 1, 0, 0, 1 }).ray(vec<3>{}, vec<3>{ 1000, 0, 0 });
	g::gfx::debug::print{ &state.my.camera }.color({ 0, 1, 0, 1 }).ray(vec<3>{}, vec<3>{ 0, 1000, 0 });
	g::gfx::debug::print{ &state.my.camera }.color({ 0, 0, 1, 1 }).ray(vec<3>{}, vec<3>{ 0, 0, 1000 });
}

ld50::renderer::renderer(g::asset::store& a, std::unordered_map<std::string, g::game::object>& m) : assets(a), object_map(m)
{
	
	
	{ // create sky sphere geometry.
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

	// draw appropriately depending on state
	switch(state.current)
	{
		case game_state::splash: draw_splash(state); break;
		case game_state::game: draw_game(state); break;
	}
}
