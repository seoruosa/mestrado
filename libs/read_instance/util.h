#pragma once

#include <vector>
#include <fstream>

std::vector<std::vector<int>> get_int_matrix(std::ifstream &file, int lines, int columns, int skip_columns);
std::vector<std::vector<float>> get_float_matrix(std::ifstream &file, int lines, int columns, int skip_columns);

template <class T>
float dist_euc(T &a, T &b);

std::vector<std::vector<float>> dist_matrix(std::vector<std::vector<int>> &a);
std::vector<std::vector<float>> dist_matrix(std::vector<std::vector<int>> &nodes, std::vector<std::vector<int>> &depots);
