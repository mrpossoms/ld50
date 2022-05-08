#include "renderer.hpp"
#include "mechanics.hpp"
#include "utils.hpp"
#include <random>

using mat4 = xmath::mat<4, 4>;

struct trajectory : public g::gfx::mesh<g::gfx::vertex::pos_norm_color>
{
	std::vector<g::gfx::vertex::pos_norm_color> verts;

	trajectory()
	{
		glGenBuffers(1, &vbo);
	}

	~trajectory()
	{
		destroy();
	}

	void set(const std::vector<vec<3>>& X, const std::vector<vec<4>>& colors)
	{
		if (verts.capacity() < X.size())
		{
			verts.reserve(X.size());
			verts.clear();
		}
		
		for (unsigned i = 0; i < X.size(); i++)
		{
			verts.push_back({ X[i], vec<3>{}, (colors[i] * 255).cast<uint8_t>() });
		}

		set_vertices(verts);
	}
};

static std::unordered_map<std::string, trajectory> TRAJECTORIES;

void ld50::renderer::draw_splash(ld50::state& state)
{

}

void ld50::renderer::render_bodies(ld50::body& b, g::game::camera& cam)
{
	auto& planet_shader = assets.shader(b.position == vec<3>{0, 0, 0} ? "pos_uv_norm.vs+star.fs" : "pos_uv_norm.vs+body.fs");

	auto itr = planet_meshes.find(b.name);
	if (itr == planet_meshes.end())
	{
		float r = b.radius;
		auto sphere = [&](const vec<3>& p) -> float { return p.magnitude() - r; };
		auto gen = [](const g::game::sdf& sdf, const vec<3>& pos) -> g::gfx::vertex::pos_norm
		{
			return { pos, pos.unit() };
		};
		vec<3> corners[] = {
			{ -r,-r,-r },
			{  r, r, r },
		};
		planet_meshes[b.name] = g::gfx::mesh_factory::from_sdf<g::gfx::vertex::pos_norm>(sphere, gen, corners);

		std::vector<vec<3>> X;
		std::vector<vec<4>> colors;

		for (float t = 0; t < 2 * M_PI; t += 0.01f)
		{
			X.push_back(quat<>::from_axis_angle({ 0, 1, 0 }, t).rotate(b.position));
			colors.push_back({ 0.5f, 0.5f, 0.5f, 1 });
		}

		TRAJECTORIES.insert({b.name, {}});
		TRAJECTORIES[b.name].set(X, colors);
	}

	auto model = mat4::translation(b.position);
	planet_meshes[b.name].using_shader(planet_shader)
		.set_camera(cam)
		["u_model"].mat4(model)
		.draw<GL_TRIANGLES>();

	TRAJECTORIES[b.name].using_shader(assets.shader("position_normal_color.vs+trajectory.fs"))
		.set_camera(cam)
		["u_model"].mat4(mat4::I())
		.draw<GL_LINE_LOOP>();

	//draw_trajectory(state, )

	for (auto& sat : b.satellites)
	{
		render_bodies(sat, cam);
	}
}

static void draw_trajectory(ld50::state& state, const vec<3>& x0, const vec<3>& dx0, unsigned prediction_points, const vec<3>& base_color={})
{

	static std::vector<vec<3>> X;
	X.clear();

	if (X.capacity() < prediction_points)
	{
		X.reserve(prediction_points);
	}

	glPointSize(3);

	auto collides = ld50::propagate_trajectory(state, x0, dx0, X);

	vec<3> color = base_color;

	if (base_color[0] == 0 && base_color[1] == 0 && base_color[2] == 0)
	{
		if (collides)
		{
			color = vec<3>{ 1, 0, 0 };
		}
		else
		{
			color = vec<3>{ 0, 1, 0 };
		}
	}

	for (unsigned i = 0; i < X.size() - 1; i++)
	{
		//auto& b = ld50::nearest_body(state, X[i]);
		auto acc = ld50::force_at_point(state, X[i], state.time);
		auto proximity_a = acc.magnitude();

		auto a = std::max<float>(proximity_a, 0.125f);

		g::gfx::debug::print{ &state.my.camera }.color({ color[0], color[1], color[2], a }).ray(X[i], X[i+1] - X[i]);
	}
}

void ld50::renderer::draw_game(ld50::state& state)
{
	auto rotation_only = state.my.camera.view();
	rotation_only[3] = { 0, 0, 0, 1 };

	auto shake = ld50::random_vec(state.generator) * state.my_player().current_thrust * std::get<float>(object_map["game.yaml"].traits()["camera_shake_amp"]);
	state.my.camera.position += shake;

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
			render_bodies(body, state.my.camera);
		}
	}

	{ // draw players
		auto& player_shader = assets.shader("pos_uv_norm.vs+body.fs");

		glDisable(GL_CULL_FACE);
		for (auto& player : state.players)
		{
			auto itr = voxel_meshes.find(player.name);
			if (itr == voxel_meshes.end())
			{
				auto& vox = assets.vox("cockpit_0.vox");
				auto com = vox.center_of_mass(true);

				voxel_meshes[player.name] = g::gfx::mesh_factory::from_voxels<g::gfx::vertex::pos_norm_color>(
					vox, 
					[&](ogt_mesh_vertex* v) -> g::gfx::vertex::pos_norm_color { // TODO: make a default from_voxels function
						
						return {
							(vec<3>{ v->pos.x, v->pos.y, v->pos.z } - com) * 0.02f,
							{ v->normal.x, v->normal.y, v->normal.z },
							{ v->color.r, v->color.g, v->color.b, v->color.a },
						};
				});
			}

			voxel_meshes[player.name].using_shader(player_shader)
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

	auto& player = state.my_player(); 

	// draw player trajectories
	{
		draw_trajectory(state, player.position, player.velocity, 500);

		auto f = ld50::force_at_point(state, player.position, state.time);
		auto thrust = f.magnitude() * std::get<float>(object_map["player-ship.yaml"].traits()["gravity_thrust_mult"]);
		draw_trajectory(state, player.position, player.velocity + player.orientation.inverse().rotate({0, 0, -thrust }), 100, {1, 1, 1});

		g::gfx::debug::print{ &state.my.camera }.color({ 1, 0, 0, 1 }).ray(vec<3>{}, vec<3>{ 1000, 0, 0 });
		g::gfx::debug::print{ &state.my.camera }.color({ 0, 1, 0, 1 }).ray(vec<3>{}, vec<3>{ 0, 1000, 0 });
		g::gfx::debug::print{ &state.my.camera }.color({ 0, 0, 1, 1 }).ray(vec<3>{}, vec<3>{ 0, 0, 1000 });
	}

	glDisable(GL_DEPTH_TEST);
	auto b = ld50::nearest_body(state, player.position);
	//text.draw(assets.shader("basic_gui.vs+basic_font.fs"), b.name, mat<4, 4>::I(), mat<4, 4>::scale({ 1.f, 1.f, 1.f }));
	
	glDisable(GL_CULL_FACE);

	
	auto w = g::gfx::width();
	auto h = g::gfx::height();

	auto welcome = "Welcome to " + b.name;
	vec<2> offset, dims;
	text.measure(welcome, dims, offset);

	vec<2> pos = { 0, h * 0.5f };
	pos -= dims * 0.5f;
	pos += offset;

	glDisable(GL_DEPTH_TEST);
	auto us = text.using_shader(assets.shader("basic_gui.vs+basic_font.fs"), welcome, [&](g::gfx::shader::usage& usage) {
		usage["u_font_color"].vec4({ 1, 1, 1, 1 })
			// .set_camera(state.my.camera)
			["u_view"].mat4(mat<4, 4>::I())
			["u_proj"].mat4(mat<4, 4>::orthographic(1, -1, w * -0.5f, w * 0.5f, h * 0.5f, h * -0.5f))
			["u_model"].mat4(mat<4, 4>::translation({ pos[0], pos[1], 0 }));
			// ["u_model"].mat4(mat<4,4>::translation(player.position));
	});
	us.draw<GL_TRIANGLES>();
	glEnable(GL_DEPTH_TEST);


	state.my.camera.position -= shake;
}

ld50::renderer::renderer(g::asset::store& a, std::unordered_map<std::string, g::game::object>& m) : 
	assets(a),
	object_map(m),
	text(a.font("UbuntuMono-B.16pt.ttf"))
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
