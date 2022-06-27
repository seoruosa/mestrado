#pragma once

#include <vector>
// #include 


using Individual = std::vector<int>;
using Tipo = Individual::value_type;

std::vector<Individual> initialize_population(int size_pop, int n_cities);
void NSGAII_mod(int size_pop, int number_generations, int number_cities, auto f, int number_obj, float mutation_rate, auto initialize_pop_func);
std::vector<Individual> generate_next_generation(std::vector<std::vector<float>> &pop_obj_val, std::vector<Individual> &pop, float mutation_rate);