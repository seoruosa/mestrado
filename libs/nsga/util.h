#pragma once

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include <ctime>
#include <chrono>
// #include <filesystem>

struct FastNonDominatedValues
{
    std::vector<std::vector<int>> fronts;
    std::vector<int> rank;
};

FastNonDominatedValues fast_non_dominated_sort(std::vector<std::vector<float>> &pop_obj_val,
                                               size_t size_pop, 
                                               int number_objectives);

std::vector<float> crowding_distance_assignment(std::vector<std::vector<float>> &pop_obj_val,
                                                int number_objectives,
                                                std::vector<int> &idx_vec);

/**
 * @brief create a function increment that could be initialized with any integer value
 * 
 */
class increment {
    private:
        int value;
    public:
        increment(): value(0) {}
        increment(int v): value(v) {}
        int operator() () { return value++; }
};

std::vector<int> generate_int_values(const int first, const int n);
std::vector<int> generate_int_values(const int n);

std::string time_now(const char *fmt);
std::string time_now();

std::string file_name(std::string const &begin, std::string const &extension);

// std::vector<std::vector<float>> evaluate_pop_obj(auto &pop, auto f);

// std::vector<std::vector<float>> evaluate_pop_obj(auto &pop, auto f, const int &obj);

// template <class Individual>
// std::vector<std::vector<float>> evaluate_pop_obj(std::vector<Individual> &pop, auto f);

// template <class Individual>
// std::vector<std::vector<float>> evaluate_pop_obj(std::vector<Individual> &pop, auto f, const int &obj);