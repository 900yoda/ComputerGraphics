#pragma once
#include <algorithm>
#include <vector>

#include "point3d.hpp"
#include "vector3d.hpp"

std::vector<qunt::point3d> getQuinticBezier(std::vector<qunt::point3d> points,
                                            int width, int height);