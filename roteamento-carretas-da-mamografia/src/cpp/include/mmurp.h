#pragma once
#include <tuple>
#include <vector>

std::tuple<float, float> splitting_chromossome(std::vector<std::vector<float>> &dist_nodes_demand,
                           std::vector<std::vector<float>> dist_nodes_depot,
                           std::vector<int> &chromossome,
                           std::vector<float> &demanda,
                           float max_load,
                           float max_dist_between_nodes,
                           int max_number_vehicles);