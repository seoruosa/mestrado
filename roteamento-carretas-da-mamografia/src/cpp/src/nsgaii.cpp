#include "nsgaii.h"
#include <iostream>
#include "playground.h"
#include "permutation.h"

#include <algorithm> //sort
#include <limits>    //numeric_limits

#include "random_generation.h"
#include "util_print.h"

void NSGAII(int size_pop, int number_generations, auto f, int number_obj, float mutation_rate)
{
    std::uniform_real_distribution<> unif(-4, 4);

    // unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
    // std::default_random_engine RND_ENGINE = std::default_random_engine(SEED);

    // initialize population
    std::vector<std::vector<float>> parent_pop(generate_random_mat(size_pop, number_obj, unif));
    std::vector<std::vector<float>> parent_pop_obj_val(evaluate_pop_obj(parent_pop, f, number_obj));

    for (int generation = 0; generation < number_generations; generation++)
    {
        // first child generation
        std::vector<std::vector<float>> child_pop = generate_next_generation(parent_pop_obj_val, parent_pop, mutation_rate);
        std::vector<std::vector<float>> child_pop_obj_val(evaluate_pop_obj(child_pop, f, number_obj));

        std::vector<std::vector<float>> next_gen_pop(child_pop);
        std::vector<std::vector<float>> next_gen_pop_obj_val(child_pop_obj_val);

        // TODO make generations an object to be easier to make the transition from one gen to other
        // create the next generation
        next_gen_pop.insert(next_gen_pop.end(), parent_pop.begin(), parent_pop.end());
        next_gen_pop_obj_val.insert(next_gen_pop_obj_val.end(), parent_pop_obj_val.begin(), parent_pop_obj_val.end());

        auto [fronts, rank] = fast_non_dominated_sort(next_gen_pop_obj_val);

        std::vector<std::vector<int>> next_pop;
        std::vector<int> next_pop_idx;

        int i = 0;
        while ((int)(next_pop_idx.size() + fronts[i].size()) <= size_pop)
        {
            next_pop_idx.insert(next_pop_idx.begin(), fronts[i].begin(), fronts[i].end());
            ++i;
        }

        std::vector<float> distances = crowding_distance_assignment(next_gen_pop_obj_val, fronts[i]);
        
        std::vector<int> perm(generate_int_values(fronts[i].size()));
        auto crowded_select = [distances, rank, fronts, i](const int &a, const int &b)
        {
            int a_mod = fronts[i][a];
            int b_mod = fronts[i][b];

            auto cmp = ((rank[a_mod] < rank[b_mod]) || ((rank[a_mod] == rank[b_mod]) && (distances[a] > distances[b])));

            return cmp;
        };

        std::sort(perm.begin(), perm.end(), crowded_select);

        for (int idx_perm = 0; ((int) next_pop_idx.size()) <= size_pop; ++idx_perm)
        {
            next_pop_idx.push_back(fronts[i][perm[idx_perm]]);
        }

        
        // populate parent pop with new generation
        for (size_t i = 0; i < parent_pop.size(); ++i)
        {
            auto idx = next_pop_idx[i];
            parent_pop[i] = next_gen_pop[idx];
        }
        parent_pop_obj_val = evaluate_pop_obj(parent_pop, f, number_obj);

    }
    
    std::cout << "x, f" << std::endl;
    for (size_t i = 0; i < parent_pop.size(); i++)
    {
        for (auto &x: parent_pop[i])
        {
            std::cout << x << "; ";
        }

        for (auto &x: parent_pop_obj_val[i])
        {
            std::cout << x << "; ";
        }

        std::cout << std::endl;
    }
}

// pop_obj_val it's the calculated objective of each individual
// returns a vector of vectors with index of individuals separated by rank levels
// TODO add dominance as a input function of the sort
// std::vector<std::vector<int>> fast_non_dominated_sort(std::vector<std::vector<float>> pop_obj_val)
// {
//     std::cout << "sort population in rank levels " << pop_obj_val.size() << std::endl;

//     return std::vector<std::vector<int>>(pop_obj_val.size(), std::vector<int>(0));
// }

std::vector<std::vector<float>> evaluate_pop_obj(auto &pop, auto f)
{
    std::vector<std::vector<float>> pop_eval;
    pop_eval.reserve(pop.size());

    for (size_t i = 0; i < pop.size(); i++)
    {
        pop_eval.push_back((f(pop[i])));
    }

    return pop_eval;
}

std::vector<std::vector<float>> evaluate_pop_obj(auto &pop, auto f, const int &obj)
{
    std::vector<std::vector<float>> pop_eval(pop.size(), std::vector<float>(obj));

    for (size_t i = 0; i < pop_eval.size(); i++)
    {
        pop_eval[i] = f(pop[i]);
    }

    return pop_eval;
}

// Gendreau2010 page 251 - Unbiased tournament selection
// void binary_tournment()
// {
//     std::cout << ">> Do binary tournament selection" << std::endl;
// }

std::vector<int> binary_tournament(std::vector<std::vector<float>> &pop_obj_val)
{
    auto pairs_to_tournament = random_pairs(pop_obj_val.size());

    auto [fronts, rank] = fast_non_dominated_sort(pop_obj_val);
    std::vector<float> distances = calculate_distance_all_pop(pop_obj_val, fronts);

    auto crowded_select = [distances](const int &i, const int &j, auto &rank)
    {
        auto cmp = ((rank[i] < rank[j]) || ((rank[i] == rank[j]) && (distances[i] > distances[j])));

        return (cmp ? i : j);
    };

    std::vector<int> parents(pop_obj_val.size());

    for (size_t i = 0; i < pop_obj_val.size(); i++)
    {
        parents[i] = crowded_select(pairs_to_tournament[i][0], pairs_to_tournament[i][1], rank);
    }

    return parents;
}

// std::vector<float> crossover(std::vector<float> &a, std::vector<float> &b, std::uniform_real_distribution<> &unif)
// {
//     std::vector<float> cut = generate_random_vec(a.size(), unif);

//     std::vector<float> child(a.size());
//     for (size_t i = 0; i < a.size(); i++)
//     {
//         child[i] = a[i] * cut[i] + b[i] * (1 - cut[i]);
//     }

//     return child;
// }

std::vector<std::vector<float>> recombination(std::vector<int> &parents, std::vector<std::vector<float>> &pop, std::uniform_real_distribution<> &unif)
{
    auto N = parents.size();
    std::vector<std::vector<float>> next_gen;
    next_gen.reserve(parents.size());

    auto crossover = [&](auto a, auto b, auto cut)
    {
        std::vector<float> child(a.size());
        for (size_t i = 0; i < a.size(); i++)
        {
            child[i] = a[i] * cut[i] + b[i] * (1 - cut[i]);
        }
        return child;
    };

    size_t i = 0;
    while (i < N - i)
    {
        auto p1 = pop[parents[i]];
        auto p2 = pop[parents[N - i - 1]];
        std::vector<float> cut = generate_random_vec(pop[0].size(), unif);

        next_gen.push_back(crossover(p1, p2, cut));
        next_gen.push_back(crossover(p2, p1, cut));

        ++i;
    }

    return next_gen;
}

std::vector<float> mutation(std::vector<float> &child, const float &mutation_rate, std::default_random_engine &engine)
{

    std::uniform_real_distribution<> unif(-0.5, 0.5);

    if (unif(engine) < mutation_rate)
    {
        std::vector<float> random_step(generate_random_vec(child.size(), unif));

        for (size_t i = 0; i < child.size(); i++)
        {
            child[i] = child[i] + random_step[i];
        }
    }

    return child;
}

void mutate(std::vector<float> &child, const float &mutation_rate, std::default_random_engine &engine)
{
    std::uniform_real_distribution<> mutate_coin(0, 1);

    // TODO escrever uma versão melhor
    if (mutate_coin(engine) < mutation_rate)
    {
        std::uniform_real_distribution<> unif(-0.5, 0.5);
        std::vector<float> random_step(generate_random_vec(child.size(), unif));

        for (size_t i = 0; i < child.size(); i++)
        {
            child[i] = child[i] + random_step[i];
        }
    }
}

std::vector<std::vector<float>> generate_next_generation(std::vector<std::vector<float>> &pop_obj_val, auto &pop, float mutation_rate)
{
    unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine RND_ENGINE = std::default_random_engine(SEED);

    std::uniform_real_distribution<> unif(0.0, 1.0);

    std::vector<int> parents = binary_tournament(pop_obj_val);
    std::vector<std::vector<float>> next_gen = recombination(parents, pop, unif);

    for (auto &child : next_gen)
    {
        mutate(child, mutation_rate, RND_ENGINE);
    }

    return next_gen;
}



// TODO create a type to deal with the return of function
std::tuple<std::vector<std::vector<int>>, std::vector<int>> fast_non_dominated_sort(std::vector<std::vector<float>> &pop_obj_val)
{
    // struct fastNonDominatedValues
    // {
    //     std::vector<std::vector<int>> fronts;
    //     std::vector<int> rank;
    // };

    int size_pop = pop_obj_val.size();
    int m = pop_obj_val.front().size(); // number of objectives

    // count how many elements dominate element i
    std::vector<int> domination_count(size_pop, 0);

    // rank of element i (starts from 0)
    std::vector<int> rank(size_pop, size_pop);

    // list the index of elements that dominates the element i
    std::vector<std::vector<int>> dominated_by(size_pop, std::vector<int>({}));

    // std::vector<std::vector<int>> fronts(size_pop, std::vector<int>({}));
    std::vector<std::vector<int>> fronts(1, std::vector<int>({}));

    // TODO use cmp function to make this calculation
    auto dominates = [m, pop_obj_val](int &i, int &j)
    {
        for (int el = 0; el < m; el++)
        {
            if (pop_obj_val[i][el] >= pop_obj_val[j][el])
            {
                return false;
            }
        }
        return true;
    };

    int i = 0;
    for (int p = 0; p < size_pop; p++)
    {
        for (int q = 0; q < size_pop; q++)
        {
            if (dominates(p, q))
            {
                dominated_by[p].push_back(q);
            }
            else if (dominates(q, p))
            {
                ++domination_count[p];
            }
        }
        // test if p is of the first front
        if (domination_count[p] == 0)
        {
            fronts[i].push_back(p);
            rank[p] = 0;
        }
    }

    while (fronts[i].size() > 0)
    {
        fronts.push_back({});
        for (auto p : fronts[i])
        {
            for (auto q : dominated_by[p])
            {
                --domination_count[q];

                if (domination_count[q] == 0)
                {
                    rank[q] = i + 1;
                    fronts[i + 1].push_back(q);
                }
            }
        }
        ++i;
    }

    return {fronts, rank};
}

std::vector<float> crowding_distance_assignment(std::vector<std::vector<float>> &L)
{
    int n = L.size();
    int m = L.front().size();

    float INF = std::numeric_limits<float>::infinity();

    std::vector<float> distance(n, 0.0);

    std::vector<int> perm = generate_int_values(n);

    for (int obj = 0; obj < m; obj++)
    {
        std::sort(perm.begin(), perm.end(), [&](auto a, auto b)
                  { return L[a][obj] < L[b][obj]; });

        float f_min = L[perm.front()][obj];
        float f_max = L[perm.back()][obj];

        distance[perm.front()] = INF;
        distance[perm.back()] = INF;

        for (size_t i = 1; i < (perm.size() - 1); i++)
        {
            if (distance[perm[i]] < INF)
            {
                distance[perm[i]] = distance[perm[i]] + (L[perm[i + 1]][obj] - L[perm[i - 1]][obj]) / (f_max - f_min);
            }
        }
    }

    return distance;
}

std::vector<float> crowding_distance_assignment(std::vector<std::vector<float>> &L, std::vector<int> &idx_vec)
{
    int n = idx_vec.size();
    int m = L.front().size();

    float INF = std::numeric_limits<float>::infinity();

    std::vector<float> distance(n, 0.0);

    std::vector<int> perm = generate_int_values(n);

    for (int obj = 0; obj < m; obj++)
    {
        std::sort(perm.begin(), perm.end(), [&](auto a, auto b)
                  { return L[idx_vec[a]][obj] < L[idx_vec[b]][obj]; });

        float f_min = L[idx_vec[perm.front()]][obj];
        float f_max = L[idx_vec[perm.back()]][obj];

        distance[perm.front()] = INF;
        distance[perm.back()] = INF;

        for (size_t i = 1; i < (perm.size() - 1); i++)
        {
            if (distance[perm[i]] < INF)
            {
                distance[perm[i]] = distance[perm[i]] + (L[idx_vec[perm[i + 1]]][obj] - L[idx_vec[perm[i - 1]]][obj]) / (f_max - f_min);
            }
        }
    }

    return distance;
}

std::vector<float> calculate_distance_all_pop(std::vector<std::vector<float>> &pop_obj_val, std::vector<std::vector<int>> &fronts)
{
    std::vector<float> distance(pop_obj_val.size(), 0.0);

    for (auto &front : fronts)
    {
        if (front.size() > 0)
        {
            std::vector<float> front_distance(crowding_distance_assignment(pop_obj_val, front));

            for (size_t i = 0; i < front.size(); i++)
            {
                distance[front[i]] = front_distance[i];
            }
        }
    }

    return distance;
}