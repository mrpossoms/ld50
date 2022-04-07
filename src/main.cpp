#include "g.h"
#include "state.hpp"
#include "renderer.hpp"

struct ld50_game : public g::core
{
	g::asset::store assets;
	ld50::state state;
	std::unique_ptr<ld50::renderer> renderer;
	std::unordered_map<std::string, g::game::object> object_map;

	ld50_game() = default;
	~ld50_game() = default;

	g::game::object& object(const std::string& name, const g::game::object::multi_trait_map traits)
	{
		auto itr = object_map.find(name);
		if (object_map.end() == itr)
		{
			object_map[name] = g::game::object(&assets, name, traits);
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

		state.players.push_back({});

		glfwSetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		return true;
	}

	virtual void update(float dt)
	{
		object("data/player-ship.yaml", {
			{ "traits", {
				{ "fuel", 100 },
				{ "thrust_per_fuel", 1 },
				{ "cam_x", 0},
				{ "cam_y", 0},
				{ "cam_z", 2},
				{ "cam_spring", 10 },
			}},
			
			{ "textures", {
				{ "diffuse", "player.diffuse.png" },
			}},
			
			{ "geometry", {
				{ "hull", "player.hull.obj"},
			}},
		});

		auto& player_traits = object("data/player-ship.yaml").traits();
		vec<3> cam_pos = { player_traits["cam_x"].number, player_traits["cam_y"].number, player_traits["cam_z"].number };

		// update the player's camera
		auto& player_ship = state.my_player();
		//state.my.camera.look_at(player_ship.position);// , player_ship.orientation.inverse().rotate({ 0, 1, 0 }));
		state.my.camera.orientation = player_ship.orientation;// .inverse();
		auto camera_orbit_target = player_ship.orientation.rotate(cam_pos);
		state.my.camera.position = state.my.camera.position.lerp(camera_orbit_target, dt * player_traits["cam_spring"].number);
		state.my_player().velocity = { 0, 0, 1 };

		static double xlast, ylast;
		float sensitivity = 0.5f;
		double xpos = 0, ypos = 0;
		auto mode = glfwGetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR);

		if (GLFW_CURSOR_DISABLED == mode)
		{
			glfwGetCursorPos(g::gfx::GLFW_WIN, &xpos, &ypos);
		}

		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_ESCAPE))
		{
			glfwSetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (glfwGetMouseButton(g::gfx::GLFW_WIN, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		{
			glfwSetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		if (glfwGetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			if (xlast != 0 || ylast != 0)
			{
				auto dx = xpos - xlast;
				auto dy = ypos - ylast;

				if (glfwGetMouseButton(g::gfx::GLFW_WIN, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
				{
					player_ship.orientation = quat<>::from_axis_angle({ 0, 1, 0 }, dx * dt * sensitivity) * player_ship.orientation;
					player_ship.orientation *= quat<>::from_axis_angle({ 1, 0, 0 }, -dy * dt * sensitivity);
				}
			}

		xlast = xpos; ylast = ypos;
		//if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_Q) == GLFW_PRESS) cam.d_roll(-dt);
		//if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_E) == GLFW_PRESS) cam.d_roll(dt);
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_LEFT) == GLFW_PRESS)  //cam.d_yaw(-dt);
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_RIGHT) == GLFW_PRESS) player_ship.orientation *= quat<>::from_axis_angle({ 0, 1, 0 }, -dt);//cam.d_yaw(dt);
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_UP) == GLFW_PRESS) player_ship.orientation *= quat<>::from_axis_angle({ 1, 0, 0 }, dt);
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_DOWN) == GLFW_PRESS) player_ship.orientation *= quat<>::from_axis_angle({ 1, 0, 0 }, -dt);


		renderer->render(state);
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
	ld50_game game;

	g::core::opts opts;

	opts.name = "ld50";
	opts.gfx.fullscreen = false;

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
