#pragma once

#include <vector>
#include <fstream>

std::vector<std::vector<int>> get_int_matrix(std::ifstream &file, int lines, int columns, int skip_columns);
std::vector<std::vector<float>> get_float_matrix(std::ifstream &file, int lines, int columns, int skip_columns);

template <class T> float dist_euc(T &a, T &b);

std::vector<std::vector<float>> dist_matrix(std::vector<std::vector<int>> &node_coor);
std::vector<std::vector<float>> dist_matrix(std::vector<std::vector<int>> &nodes, std::vector<std::vector<int>> &depots);

void read_instance(std::vector<std::vector<float>> &dist_nodes_nodes, 
                   std::vector<std::vector<float>> &dist_depots_nodes,
                   std::vector<float> &demand, 
                   int &capacity, 
                   std::vector<int> &number_vehicles, 
                   float &max_travel_dist,
                   const std::string &name, 
                   const std::string &filepath);