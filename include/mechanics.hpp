#pragma once
#include <g.h>
#include "state.hpp"

using namespace xmath;

namespace ld50
{

vec<3> random_vec(std::default_random_engine& generator, const vec<3>& min = { -1, -1, -1 }, const vec<3> max = { 1, 1, 1 });

float nearest_body_dist(const ld50::state& state, const vec<3>& pos);

vec<3> force_at_point(const ld50::state& state, const vec<3>& pos, float t);

vec<3> force_at_point_branch(ld50::state& state, const vec<3>& pos, float t);

void handle_controls(ld50::state& state, std::unordered_map<std::string, g::game::object>& objects, float dt);

void update_body_velocities(ld50::state& state, float dt);

void update_body_positions(ld50::state& state, ld50::body& parent, const vec<3>& parent_pos);

void populate_solar_system(ld50::state& state, unsigned bodies, unsigned seed);

ld50::body& biggest_influence(ld50::state& state, const vec<3>& pos);

} // namespace ld50