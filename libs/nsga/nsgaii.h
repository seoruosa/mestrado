#pragma once

#include <vector>
#include <string>

#include "util.h"
#include "../utils/performance.h"

#include <random>
#include <algorithm>

template <class Individual>
struct Solution
{
    std::vector<Individual> pop;
    std::vector<std::vector<float>> pop_obj_val;
};

template <class Individual>
class NSGAII
{
protected:
    unsigned seed;
    int population_size;
    int number_generations;
    float mutation_rate;
    int number_objectives;
    std::default_random_engine random_engine;

public:
    NSGAII(int population_size, int number_generations, float mutation_rate,
           int number_objectives, unsigned seed);
    ~NSGAII();

    virtual std::vector<float> evaluate(Individual &ind) = 0;
    virtual std::vector<Individual> initialize_population() = 0;
    virtual std::vector<int> parent_selection(int number_parents) = 0; // indexes of parents
    // virtual Individual &crossover(Individual &p1, Individual &p2, std::vector<int> &cuts) = 0;
    virtual std::vector<Individual> recombination(std::vector<int> &parents,
                                                  std::vector<Individual> &pop) = 0;
    virtual Individual &mutate(Individual &ind) = 0;

    std::vector<std::vector<float>> evaluate_pop_obj(std::vector<Individual> &pop);
    std::vector<Individual> generate_next_generation(std::vector<std::vector<float>> &pop_obj_val,
                                                     std::vector<Individual> &pop); // remover
    std::vector<int> sort_order_of_front_by_crowding_distance(std::vector<int> &front,
                                                              std::vector<int> &rank,
                                                              std::vector<std::vector<float>> &next_gen_pop_obj_val);
    bool coin(float true_prob);
    // std::vector<int> generate_random_integers(size_t vector_size, int min, int max);

    Solution<Individual> run_default();
};

template <class Individual>
NSGAII<Individual>::NSGAII(int population_size, int number_generations, float mutation_rate,
                           int number_objectives, unsigned seed) : population_size(population_size),
                                                                   number_generations(number_generations),
                                                                   mutation_rate(mutation_rate),
                                                                   number_objectives(number_objectives),
                                                                   seed(seed),
                                                                   random_engine(std::default_random_engine(seed))
{
}

template <class Individual>
NSGAII<Individual>::~NSGAII() {}

template <class Individual>
std::vector<std::vector<float>> NSGAII<Individual>::evaluate_pop_obj(std::vector<Individual> &pop)
{
    std::vector<std::vector<float>> pop_eval;
    pop_eval.reserve(pop.size());

    for (size_t i = 0; i < pop.size(); i++)
    {
        pop_eval.push_back(evaluate(pop[i]));
    }

    return pop_eval;
}

template <class Individual>
std::vector<Individual> NSGAII<Individual>::generate_next_generation(std::vector<std::vector<float>> &pop_obj_val,
                                                                     std::vector<Individual> &pop)
{
    std::vector<int> parents = parent_selection(2 * population_size);
    std::vector<Individual> next_gen = recombination(parents, pop);

    for (auto &child : next_gen)
    {
        if (coin(mutation_rate))
        {
            mutate(child);
        }
    }

    return next_gen;
}

template <class Individual>
bool NSGAII<Individual>::coin(float true_prob)
{
    std::uniform_real_distribution<> coin_generator(0, 1);

    return (coin_generator(random_engine) < true_prob);
}

template <class Individual>
std::vector<int> NSGAII<Individual>::sort_order_of_front_by_crowding_distance(
    std::vector<int> &front,
    std::vector<int> &rank,
    std::vector<std::vector<float>> &next_gen_pop_obj_val)
{
    auto distances = crowding_distance_assignment(next_gen_pop_obj_val, number_objectives,
                                             front);
    auto perm = generate_int_values(front.size());

    auto crowded_select = [distances, rank, front](const int &a, const int &b)
    {
        int a_mod = front[a];
        int b_mod = front[b];

        auto cmp = ((rank[a_mod] < rank[b_mod]) ||
                    ((rank[a_mod] == rank[b_mod]) && (distances[a] > distances[b])));

        return cmp;
    };

    std::sort(perm.begin(), perm.end(), crowded_select);

    return perm;
}
// template <class Individual>
// std::vector<int> NSGAII<Individual>::generate_random_integers(size_t vector_size,
//                                                               int min,
//                                                               int max)
// {
//     std::uniform_int_distribution<> distrib(min, max);

//     std::vector<int> vec;
//     vec.reserve(vector_size);

//     for (size_t i = 0; i < vector_size; i++)
//     {
//         vec.push_back(distrib(random_engine));
//     }

//     return vec;
// }

template <class Individual>
Solution<Individual> NSGAII<Individual>::run_default()
{
    std::ofstream MEASUREMENT_FILE(file_name("measurements", "txt"), std::ios::ate);

    // initialize timers
    auto start = perf::time::start();
    auto start_gen = start;
    auto time_start = start;

    auto time_duration = perf::time::duration(time_start);

    // initialize used vectors
    std::vector<Individual> child_pop;
    std::vector<std::vector<float>> child_pop_obj_val;

    std::vector<Individual> next_gen_pop;
    std::vector<std::vector<float>> next_gen_pop_obj_val;
    std::vector<int> next_pop_idx;

    std::vector<int> perm;

    std::vector<Individual> parent_pop(initialize_population());
    std::vector<std::vector<float>> parent_pop_obj_val(evaluate_pop_obj(parent_pop));

    for (int generation = 0; generation < number_generations; generation++)
    {
        perf::save_gen(generation, MEASUREMENT_FILE);

        start_gen = perf::time::start();
        time_start = perf::time::start();

        child_pop = generate_next_generation(parent_pop_obj_val, parent_pop);
        time_duration = perf::time::duration(time_start);

        perf::save_measurement("next_generation:generate", time_duration, MEASUREMENT_FILE);

        time_start = perf::time::start();
        child_pop_obj_val = evaluate_pop_obj(child_pop);

        next_gen_pop = child_pop;
        next_gen_pop_obj_val = child_pop_obj_val;

        time_duration = perf::time::duration(time_start);
        perf::save_measurement("next_generation:evaluate", time_duration, MEASUREMENT_FILE);

        // create the next generation
        time_start = perf::time::start();

        next_gen_pop.insert(next_gen_pop.end(), parent_pop.begin(), parent_pop.end());
        next_gen_pop_obj_val.insert(next_gen_pop_obj_val.end(), parent_pop_obj_val.begin(),
                                    parent_pop_obj_val.end());

        time_duration = perf::time::duration(time_start);
        perf::save_measurement("next_generation:insert", time_duration, MEASUREMENT_FILE);

        time_start = perf::time::start();
        auto [fronts, rank] = fast_non_dominated_sort(next_gen_pop_obj_val, population_size,
                                                      number_objectives);

        time_duration = perf::time::duration(time_start);
        perf::save_measurement("next_generation:fast_sort", time_duration, MEASUREMENT_FILE);

        time_start = perf::time::start();

        next_pop_idx.clear();

        int i = 0;
        while ((next_pop_idx.size() + fronts[i].size()) <= population_size)
        {
            next_pop_idx.insert(next_pop_idx.begin(), fronts[i].begin(), fronts[i].end());
            ++i;
        }

        time_duration = perf::time::duration(time_start);
        perf::save_measurement("next_generation:select_except_last", time_duration, MEASUREMENT_FILE);

        time_start = perf::time::start();
        
        perm = sort_order_of_front_by_crowding_distance(fronts[i], rank, next_gen_pop_obj_val);

        for (int idx_perm = 0; next_pop_idx.size() <= population_size; ++idx_perm)
        {
            next_pop_idx.push_back(fronts[i][perm[idx_perm]]);
        }

        // populate parent pop with new generation
        for (size_t i = 0; i < parent_pop.size(); ++i)
        {
            auto idx = next_pop_idx[i];
            parent_pop[i] = next_gen_pop[idx];
            parent_pop_obj_val[i] = next_gen_pop_obj_val[idx];
        }

        time_duration = perf::time::duration(time_start);
        perf::save_measurement("next_generation:crowd_sort_and_finish", time_duration, MEASUREMENT_FILE);
    }

    time_duration = perf::time::duration(start);
    perf::save_measurement("nsga:total", time_duration, MEASUREMENT_FILE);

    // END_FILE << "DURACAO : " << (time_duration.count()/1.e6) << std::endl;

    // print_solution_csv(parent_pop, parent_pop_obj_val, END_FILE);

    MEASUREMENT_FILE.close();

    return {parent_pop, parent_pop_obj_val};
}

void blabla();