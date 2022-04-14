#pragma once
#include <g.h>
#include "state.hpp"

using namespace xmath;

namespace ld50
{

vec<3> force_at_point(const ld50::state& state, const vec<3>& pos, float t);

void handle_controls(ld50::state& state, std::unordered_map<std::string, g::game::object>& objects, float dt);

void update_body_velocities(ld50::state& state, float dt);

void update_body_positions(ld50::state& state, ld50::body& parent, const vec<3>& parent_pos);

void populate_solar_system(ld50::state& state, unsigned bodies, unsigned seed);

} // namespace ld50