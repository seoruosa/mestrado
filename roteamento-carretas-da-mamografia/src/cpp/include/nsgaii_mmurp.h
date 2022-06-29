#pragma once

#include <vector>
// #include 

struct NSGAII_MMURP_Params
{
    std::string instance_path;
    float max_dist_between_nodes;
    int max_number_vehicles;
    int size_pop;
    int number_generations;
    float mutation_rate;
};

using Individual = std::vector<int>;
using Tipo = Individual::value_type;

std::vector<Individual> initialize_population(int size_pop, int n_cities);
std::tuple<std::vector<Individual>, std::vector<std::vector<float>>> NSGAII_mod(int size_pop, int number_generations, int number_cities, auto f, int number_obj, float mutation_rate, auto initialize_pop_func);
std::vector<Individual> generate_next_generation(std::vector<std::vector<float>> &pop_obj_val, std::vector<Individual> &pop, float mutation_rate);
NSGAII_MMURP_Params read_input(const int &argc, const char *argv[]);