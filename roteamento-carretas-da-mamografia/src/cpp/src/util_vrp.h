#pragma once
#include <vector>

float open_route_distance(std::vector<std::vector<float>>& dist, std::vector<int>& route);
std::vector<int> crossover_OX(std::vector<int> &P1, std::vector<int> &P2, int cut_begin, int cut_end);