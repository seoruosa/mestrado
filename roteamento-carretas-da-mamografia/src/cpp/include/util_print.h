#pragma once
#include <string>
#include <vector>

template <class T> void print_vec(T &vec, const std::string sep);
template <class T> void print_vec(T &vec);

template <class T> void print_mat(T &mat);

void starline(int num);
void starline();

void print_solution_csv(std::vector<std::vector<int>> pop, std::vector<std::vector<float>> pop_obj_val);
std::string time_now(const char *fmt);
std::string time_now();