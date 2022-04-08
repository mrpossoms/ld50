#pragma once
#include "state.hpp"

using namespace xmath;

namespace ld50
{

vec<3> acceleration_at_point(const ld50::state& state, const vec<3>& pos, float t);

} // namespace ld50