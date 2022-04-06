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

	const g::game::object& object(const std::string& name, const g::game::object::multi_trait_map traits)
	{
		auto itr = object_map.find(name);
		if (object_map.end() == itr)
		{
			object_map[name] = g::game::object(&assets, name, traits);
		}

		return object_map[name];
	}

	const g::game::object& object(const std::string& name)
	{
		return object_map[name];
	}

	virtual bool initialize()
	{
		renderer = std::make_unique<ld50::renderer>(assets);

		state.players.push_back({});

		return true;
	}

	virtual void update(float dt)
	{
		object("data/player-ship.yaml", {
			{ "traits", {
				{ "fuel", 100 },
				{ "thrust_per_fuel", 1 },
			}},
			
			{ "textures", {
				{ "diffuse", "player.diffuse.png" },
			}},
			
			{ "geometry", {
				{ "hull", "player.hull.obj"},
			}},
		});

		const auto speed = 4.0f;
		auto& cam = state.my.camera;
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_W) == GLFW_PRESS) cam.position += cam.forward() * dt * speed;
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_S) == GLFW_PRESS) cam.position += cam.forward() * -dt * speed;
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_A) == GLFW_PRESS) cam.position += cam.left() * -dt * speed;
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_D) == GLFW_PRESS) cam.position += cam.left() * dt * speed;
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_Q) == GLFW_PRESS) cam.d_roll(-dt);
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_E) == GLFW_PRESS) cam.d_roll(dt);
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_LEFT) == GLFW_PRESS) cam.d_yaw(-dt);
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_RIGHT) == GLFW_PRESS) cam.d_yaw(dt);
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_UP) == GLFW_PRESS) cam.d_pitch(dt);
		if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_DOWN) == GLFW_PRESS) cam.d_pitch(-dt);


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
