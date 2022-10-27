#pragma once

#include <vector>

struct NSGAII_MMURP_Params
{
    std::string instance_path;
    int size_pop;
    int number_generations;
    float mutation_rate;
};

using Individual = std::vector<int>;
using Tipo = Individual::value_type;

std::vector<Individual> initialize_population(int size_pop, int n_cities);

std::tuple<std::vector<Individual>, std::vector<std::vector<float>>> NSGAII_mod(int size_pop, int number_generations, int number_cities, 
    auto f, int number_obj, float mutation_rate, auto initialize_pop_func, std::ostream &BEGIN_FILE, std::ostream &END_FILE);
std::vector<Individual> generate_next_generation(std::vector<std::vector<float>> &pop_obj_val, std::vector<Individual> &pop, float mutation_rate);
NSGAII_MMURP_Params read_input(const int &argc, const char *argv[]);

void print_params(NSGAII_MMURP_Params config, int capacity, const std::string &name, std::ostream &out);
std::string instance_name(std::string const &begin, int n, int m, int Q, int max_veh);
std::string instance_name(std::string const &begin, int n, int m, int Q, const std::vector<int> &max_veh);
std::string file_name(std::string const &begin, int n, int m, int Q, int max_veh, std::string const &extension);
std::string file_name(std::string const &begin, std::string const &extension);
float max_dist(std::vector<std::vector<float>> &dist_nodes_mat, std::vector<std::vector<float>> &dist_depots_nodes_mat);
std::string ref_point(std::vector<std::vector<float>> &dist_nodes_mat, std::vector<std::vector<float>> &dist_depots_nodes_mat);
