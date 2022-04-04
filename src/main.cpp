#include "g.h"
#include "state.hpp"
#include "renderer.hpp"

struct ld50_game : public g::core
{
	g::asset::store assets;
	ld50::state state;
	std::unique_ptr<ld50::renderer> renderer;

	ld50_game() = default;
	~ld50_game() = default;

	virtual bool initialize()
	{
		renderer = std::make_unique<ld50::renderer>(assets);

		return true;
	}

	virtual void update(float dt)
	{
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
