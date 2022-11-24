#include "util.h"
#include <limits>
#include <algorithm>

FastNonDominatedValues fast_non_dominated_sort(std::vector<std::vector<float>> &pop_obj_val,
                                               size_t size_pop, 
                                               int number_objectives)
{
    // count how many elements dominate element i
    std::vector<int> domination_count(size_pop, 0);

    // rank of element i (starts from 0)
    std::vector<int> rank(size_pop, size_pop);

    // list the index of elements that dominates the element i
    std::vector<std::vector<int>> dominated_by(size_pop, std::vector<int>({}));

    std::vector<std::vector<int>> fronts(1, std::vector<int>({}));

    // TODO use cmp function to make this calculation
    auto dominates = [number_objectives, pop_obj_val](size_t &i, size_t &j)
    {
        for (int el = 0; el < number_objectives; el++)
        {
            if (pop_obj_val[i][el] >= pop_obj_val[j][el])
            {
                return false;
            }
        }
        return true;
    };

    size_t i = 0;
    for (size_t p = 0; p < size_pop; p++)
    {
        for (size_t q = 0; q < size_pop; q++)
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

std::vector<float> crowding_distance_assignment(std::vector<std::vector<float>> &pop_obj_val,
                                                int number_objectives,
                                                std::vector<int> &idx_vec)
{
    int n = idx_vec.size();

    float INF = std::numeric_limits<float>::infinity();

    std::vector<float> distance(n, 0.0);

    std::vector<int> perm = generate_int_values(n);

    for (int obj = 0; obj < number_objectives; obj++)
    {
        std::sort(perm.begin(), perm.end(), [&](auto a, auto b)
                  { return pop_obj_val[idx_vec[a]][obj] < pop_obj_val[idx_vec[b]][obj]; });

        float f_min = pop_obj_val[idx_vec[perm.front()]][obj];
        float f_max = pop_obj_val[idx_vec[perm.back()]][obj];

        distance[perm.front()] = INF;
        distance[perm.back()] = INF;

        for (size_t i = 1; i < (perm.size() - 1); i++)
        {
            if (distance[perm[i]] < INF)
            {
                distance[perm[i]] = distance[perm[i]] + (pop_obj_val[idx_vec[perm[i + 1]]][obj] - pop_obj_val[idx_vec[perm[i - 1]]][obj]) / (f_max - f_min);
            }
        }
    }

    return distance;
}

std::vector<int> generate_int_values(const int first, const int n)
{
    std::vector<int> vec(n);
    std::generate_n(vec.begin(), n, increment(first));
    return vec;
}

std::vector<int> generate_int_values(const int n)
{
    return generate_int_values(0, n);
}

std::string time_now(const char *fmt)
{
    // https://www.programiz.com/cpp-programming/library-function/ctime/strftime
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    char date_string[100];

    std::strftime(date_string, 100, fmt, std::localtime(&time));
    std::string out(date_string);

    return out;
};

std::string time_now()
{
    return time_now("%Y-%m-%d_%H:%M:%S");
}

std::string file_name(std::string const &begin, std::string const &extension)
{
    std::ostringstream name;

    name << begin << "-" << time_now("%Y%m%d%H%M%S") << "." << extension;

    return name.str();
}

// // template <class Individual>
// // std::vector<std::vector<float>> evaluate_pop_obj(std::vector<Individual> &pop, auto f)
// std::vector<std::vector<float>> evaluate_pop_obj(auto &pop, auto f)
// {
//     std::vector<std::vector<float>> pop_eval;
//     pop_eval.reserve(pop.size());

//     for (size_t i = 0; i < pop.size(); i++)
//     {
//         pop_eval.push_back(f(pop[i]));
//     }

//     return pop_eval;
// }

// // template <class Individual>
// // std::vector<std::vector<float>> evaluate_pop_obj(std::vector<Individual> &pop, auto f, const int &obj)

// std::vector<std::vector<float>> evaluate_pop_obj(auto &pop, auto f, const int &obj)
// {
//     std::vector<std::vector<float>> pop_eval(pop.size(), std::vector<float>(obj));

//     for (size_t i = 0; i < pop_eval.size(); i++)
//     {
//         pop_eval[i] = f(pop[i]);
//     }

//     return pop_eval;
// }