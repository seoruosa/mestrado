#pragma once

#include <vector>
#include <tuple>
#include <random>

std::tuple<std::vector<std::vector<int>>, std::vector<int>> fast_non_dominated_sort(std::vector<std::vector<float>> &pop_obj_val);

std::vector<std::vector<float>> evaluate_pop_obj(auto &pop, auto f);
std::vector<std::vector<float>> evaluate_pop_obj(auto &pop, auto f, const int& obj);

// void generate_next_generation(float mutation_rate);
std::vector<std::vector<float>> generate_next_generation(std::vector<std::vector<float>> &pop, float mutation_rate);
std::vector<std::vector<float>> generate_next_generation(std::vector<std::vector<float>> &pop_obj_val, auto &pop, float mutation_rate);


std::vector<float> mutation(std::vector<float> &child, const float &mutation_rate, std::default_random_engine &engine);

void mutate(std::vector<float> &child, const float &mutation_rate, std::default_random_engine &engine);

std::vector<std::vector<float>> recombination(std::vector<int> &parents, std::vector<std::vector<float>> &pop, std::uniform_real_distribution<> &unif);
// std::vector<float> crossover(std::vector<float> &a, std::vector<float> &b, std::uniform_real_distribution<> &unif);

std::vector<int> binary_tournament(std::vector<std::vector<float>> &pop_obj_val);

std::vector<float> crowding_distance_assignment(std::vector<std::vector<float>> &L);
std::vector<float> crowding_distance_assignment(std::vector<std::vector<float>> &L, std::vector<int> &idx_vec);
std::vector<float> calculate_distance_all_pop(std::vector<std::vector<float>> &pop_obj_val, std::vector<std::vector<int>> &fronts);

void NSGAII(int size_pop, int number_generations, auto f, int number_obj, float mutation_rate);


// *******************************************************************************************************************************************
// ********************************************* DOING SOME MODIFICATIONS ON FUNCTIONS *******************************************************
// *******************************************************************************************************************************************

// std::vector<std::vector<float>> initialize_population(int size_pop, int number_obj, float lower, float upper);
// void NSGAII_mod(int size_pop, int number_generations, auto f, int number_obj, float mutation_rate, auto initialize_pop_func);