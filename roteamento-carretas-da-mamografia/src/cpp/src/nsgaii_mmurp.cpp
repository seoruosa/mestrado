#include "nsgaii.h"
#include "nsgaii_mmurp.h"

#include <iostream>
#include "playground.h"
#include "permutation.cpp"

#include <algorithm> //sort
#include <limits>    //numeric_limits

// #include "random_generation.h"
#include "util_print.h"

#include "read_instance.h"
#include "util_print.cpp"

// #include "seeds.h" // RND_ENGINE
#include "mmurp.h"
#include "util_vrp.h"

// unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
// auto RND_ENGINE = std::default_random_engine(SEED);

int main()
{
    // auto kur = [](std::vector<float> x){

    //     float f1 = 0.0;
    //     float f2 = 0.0;
    //     auto m = x.size();

    //     for (size_t i = 0; i < m - 1; i++)
    //         f1 = f1 - 10* std::exp(-0.2 * std::sqrt(x[i]*x[i] + x[i + 1]*x[i + 1]));

    //     for (size_t i = 0; i < m; i++)
    //         f2 = f2 + std::pow(std::abs(x[i]), 0.8) + 5*std::sin(x[i]*x[i]*x[i]);

    //     return std::vector<float>({f1, f2});
    // };

    // float lower = -4.0;
    // float upper = 4.0;

    // auto init_pop = [&](int size_pop, int number_obj){return initialize_population(size_pop, number_obj, lower, upper);};

    // NSGAII_mod(50, 100, kur, 3, 0.1, init_pop);

    std::string filepath = "../../../Instancias/Vrp-Set-A/A-n50-m4-Q80.vrp";

    int CAPACITY;

    std::vector<std::vector<int>> nodes_coord;
    std::vector<std::vector<int>> demand_mat;
    std::vector<std::vector<int>> depots_coord;

    read_instance(nodes_coord, demand_mat, depots_coord, CAPACITY, filepath);

    std::vector<std::vector<float>> dist_nodes_mat;
    std::vector<std::vector<float>> dist_depots_nodes_mat;

    dist_nodes_mat = dist_matrix(nodes_coord);
    dist_depots_nodes_mat = dist_matrix(nodes_coord, depots_coord);

    // TODO correct the type of return of demand_mat of read_instance function
    std::vector<float> demand;
    demand.reserve(demand_mat.size());
    for (auto &demand_node : demand_mat)
    {
        demand.push_back(demand_node[0]);
    }
    

    auto calc = [&](Individual x){

        auto [demanda_atendida, dist_percorrida] = splitting_chromossome(dist_nodes_mat, dist_depots_nodes_mat, x, demand, CAPACITY, 200, 10);

        return std::vector<float>({dist_percorrida, -demanda_atendida});
    };

    // auto a = initialize_population(10, 15);

    // print_mat(a);
    // print_mat(dist_depots_nodes_mat);
    // std::cout << "***************************************" << std::endl;
    // print_mat(dist_nodes_mat);

    // NSGAII_mod(50, 100, kur, 3, 0.1, init_pop);
    NSGAII_mod(50, 1000, demand.size(), calc, 2, 0.1, initialize_population);
}

std::vector<Individual> initialize_population(int size_pop, int n_cities)
{

    std::vector<Individual> pop(size_pop, generate_int_values(n_cities));

    for (auto &ind : pop)
    {
        std::shuffle(ind.begin(), ind.end(), RND_ENGINE);
    }

    return pop;
}

void NSGAII_mod(int size_pop, int number_generations, int number_cities, auto f, int number_obj, float mutation_rate, auto initialize_pop_func)
{
    // initialize population
    // TODO need to create a initialization for MMURP
    // TODO parent_pop will be necessary to change type
    std::vector<Individual> parent_pop(initialize_pop_func(size_pop, number_cities));

    // TODO parent_pop_obj_val change type
    // TODO evaluate_pop_obj need to accept different parent_pop input type (prob not, because it's using auto...)
    std::vector<std::vector<float>> parent_pop_obj_val(evaluate_pop_obj(parent_pop, f, number_obj));

    // print_mat(parent_pop);
    // std::cout << "*****************************************" << std::endl;
    // print_mat(parent_pop_obj_val);

    for (int generation = 0; generation < number_generations; generation++)
    {
        // first child generation
        // TODO child_pop need to change the type
        // TODO should be a function input
        std::vector<Individual> child_pop = generate_next_generation(parent_pop_obj_val, parent_pop, mutation_rate);
        // print_mat(child_pop);
        // return;
        // std::vector<Individual> child_pop = generate_next_gen(parent_pop_obj_val, parent_pop, mutation_rate);
        std::vector<std::vector<float>> child_pop_obj_val(evaluate_pop_obj(child_pop, f, number_obj));

        std::vector<Individual> next_gen_pop(child_pop);
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
        // TODO crowded select could sort direct the fronts[i]???
        auto crowded_select = [distances, rank, fronts, i](const int &a, const int &b)
        {
            int a_mod = fronts[i][a];
            int b_mod = fronts[i][b];

            auto cmp = ((rank[a_mod] < rank[b_mod]) || ((rank[a_mod] == rank[b_mod]) && (distances[a] > distances[b])));

            return cmp;
        };

        std::sort(perm.begin(), perm.end(), crowded_select);

        for (int idx_perm = 0; ((int)next_pop_idx.size()) <= size_pop; ++idx_perm)
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
        for (auto &x : parent_pop[i])
        {
            std::cout << x << "; ";
        }

        for (auto &x : parent_pop_obj_val[i])
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

// TODO pop should be related to the problem
// TODO the crossover function should be specific to problem (could be a input of function)
std::vector<Individual> recombination(std::vector<int> &parents, std::vector<Individual> &pop, std::uniform_real_distribution<> &unif)
{
    int size = pop[0].size();
    auto N = pop.size();

    std::vector<Individual> next_gen;
    next_gen.reserve(parents.size());

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, size - 1);

    size_t i = 0;
    while (i < N - i)
    {
        auto p1 = pop[parents[i]];
        auto p2 = pop[parents[N - i - 1]];

        int cut1 = distrib(gen);
        int cut2 = distrib(gen);

        // TODO add this logic inside crossover_OX
        int cut_left = std::min(cut1, cut2);
        int cut_right = std::max(cut1, cut2);

        // std::cout << "CUT-> left:" << cut_left << " RIGHT: " << cut_right << std::endl;

        next_gen.push_back(crossover_OX(p1, p2, cut_left, cut_right));
        next_gen.push_back(crossover_OX(p2, p1, cut_left, cut_right));

        ++i;
        // std::cout << "POP_SIZE: " << pop.size() << " NEXT_GEN_SIZE: " << next_gen.size() << std::endl;
    }

    return next_gen;
}

// // TODO specific to the problem
// std::vector<float> mutation(std::vector<float> &child, const float &mutation_rate, std::default_random_engine &engine)
// {

//     std::uniform_real_distribution<> unif(-0.5, 0.5);

//     if (unif(engine) < mutation_rate)
//     {
//         std::vector<float> random_step(generate_random_vec(child.size(), unif));

//         for (size_t i = 0; i < child.size(); i++)
//         {
//             child[i] = child[i] + random_step[i];
//         }
//     }

//     return child;
// }

// TODO specific to the problem
void mutate(Individual &child, const float &mutation_rate, std::default_random_engine &engine)
{
    std::uniform_real_distribution<> mutate_coin(0, 1);

    if (mutate_coin(engine) < mutation_rate)
    {
        // https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(0, child.size() - 1);
        int pos1 = distrib(gen);
        int pos2 = distrib(gen);

        std::iter_swap(&(child[pos1]), &(child[pos2]));
    }
}

// TODO problem specific
std::vector<Individual> generate_next_generation(std::vector<std::vector<float>> &pop_obj_val, std::vector<Individual> &pop, float mutation_rate)
{
    unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine RND_ENGINE = std::default_random_engine(SEED);

    std::uniform_real_distribution<> unif(0.0, 1.0);

    std::vector<int> parents = binary_tournament(pop_obj_val);
    // std::cout << "PARENTS *****************" << std::endl;
    // print_vec(parents);
    std::vector<Individual> next_gen = recombination(parents, pop, unif);
    // std::cout << "NEXT_GEN *****************" << std::endl;
    // print_mat(next_gen);

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