#pragma once

#include <random>
#include <vector>
#include <chrono>

std::vector<float> generate_random_vec(const int &size, std::uniform_real_distribution<> &unif);

std::vector<float> generate_random_vec(const int &size);
std::vector<float> generate_random_vec(const int &size, const float &min, const float &max);

void generate_random_mat(std::vector<std::vector<float>> &mat, std::uniform_real_distribution<> &unif);
std::vector<std::vector<float>> generate_random_mat(const int &rows, const int &columns, std::uniform_real_distribution<> &unif);
