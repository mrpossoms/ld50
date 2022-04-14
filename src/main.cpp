#include "g.h"
#include "state.hpp"
#include "renderer.hpp"
#include "mechanics.hpp"

#undef near
#undef far

float SCROLL_D_Y = 0;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	SCROLL_D_Y = yoffset;
}


struct ld50_game : public g::core
{
	g::asset::store assets;
	ld50::state state;
	std::unique_ptr<ld50::renderer> renderer;
	std::unordered_map<std::string, g::game::object> object_map;
	
	ld50_game(const char* root) : assets(root ? std::string(root) : "data") {}
	~ld50_game() = default;

	g::game::object& object(const std::string& name, const g::game::object::multi_trait_map traits)
	{
		auto itr = object_map.find(name);
		if (object_map.end() == itr)
		{
			object_map[name] = g::game::object(&assets, assets.get_root_path() + "/" + name, traits);
		}

		return object_map[name];
	}

	g::game::object& object(const std::string& name)
	{
		return object_map[name];
	}

	virtual bool initialize()
	{
		renderer = std::make_unique<ld50::renderer>(assets, object_map);


		//ld50::body star;
		//star.mass = 100;
		//star.radius = 10;
		//
		//ld50::body planet;
		//planet.model_name = "planet";
		//planet.orbit.a = 200;
		//planet.mass = 100;
		//planet.radius = 2;
		//star.satellites.push_back(planet);
		//state.bodies.push_back(star);

		ld50::populate_solar_system(state, 10, 0);
		ld50::update_body_positions(state, state.bodies[0], {});
		auto& start_planet = state.bodies[0].satellites[0];

		state.my.camera.near = 1.f;
		state.my.camera.far = 10000.f;

		ld50::player p;
		auto r = start_planet.radius * 2;
		p.position = start_planet.position + vec<3>{ r, 0, 0 };
		p.linear_momentum = { 0, 0, -sqrt(start_planet.mass / r) };
		state.players.push_back(p);

		state.current = ld50::game_state::game;

		glfwSetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetScrollCallback(g::gfx::GLFW_WIN, scroll_callback);

		// ld50::kepler k;
		// k.a = 20;
		// k.e = 0.5;

		// for (float t = 10; t < 20; t += 1)
		// {
		// 	k.position_at(100, t);
		// }

		//k.position_at(100, 1.1);

		return true;
	}

	virtual void update(float dt)
	{

		object("player-ship.yaml", {
			{ "traits", {
				{ "fuel", 100 },
				{ "thrust_per_fuel", 2 },
				{ "cam_x", 0},
				{ "cam_y", 20},
				{ "cam_z", 20},
				{ "gravity_thrust_mult", 10 },
				{ "cam_spring", 10 },
			}},
			
			{ "textures", {
				{ "diffuse", "player.diffuse.png" },
			}},
			
			{ "geometry", {
				{ "hull", "player.hull.obj"},
			}},
		});

		state.my.zoom += SCROLL_D_Y * 0.1f;
		SCROLL_D_Y = 0;

		auto& player_traits = object("player-ship.yaml").traits();
		auto cam_x = std::get<float>(player_traits["cam_x"]);
		auto cam_y = std::get<float>(player_traits["cam_y"]);
		auto cam_z = std::get<float>(player_traits["cam_z"]);
		auto cam_pos = vec<3>{ cam_x, cam_y, cam_z } * state.my.zoom;

		// update the player's camera
		auto& player_ship = state.my_player();
		auto camera_orbit_target = state.my.camera.orientation.rotate(cam_pos) + player_ship.position;
		state.my.camera.position = state.my.camera.position.lerp(camera_orbit_target, dt * std::get<float>(player_traits["cam_spring"]));
		
		ld50::handle_controls(state, object_map, dt);


		// ld50::update_body_velocities(state, dt);
		ld50::update_body_positions(state, state.bodies[0], {});

		player_ship.dyn_apply_global_force(player_ship.position, ld50::force_at_point(state, player_ship.position, state.time));
		player_ship.dyn_step(dt);

		renderer->render(state);

		state.time += dt;
	}

};

#ifdef __EMSCRIPTEN__
EM_JS(int, canvas_get_width, (), {
  return document.getElementById('canvas').width;
});

EM_JS(int, canvas_get_height, (), {
  return document.getElementById('canvas').height;
});
#endif

int main (int argc, const char* argv[])
{
	ld50_game game(argc > 0 ? argv[1] : nullptr);

	g::core::opts opts;

	opts.name = "ld50";
	opts.gfx.fullscreen = false;
	// opts.gfx.width = 1024;
	// opts.gfx.height = 768;

#ifdef __EMSCRIPTEN__
	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	opts.gfx.width = canvas_get_width();
	opts.gfx.height = canvas_get_height();
#else
	opts.gfx.fullscreen = false;
#endif

	game.start(opts);

	return 0;
}
