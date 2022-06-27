#include <iostream>

#include "nsgaii.cpp"
#include "nsgaii_mmurp.h"

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

int main()
{
    std::string filepath = "../../../Instancias/Vrp-Set-A/A-n50-m4-Q80.vrp";
    // std::string filepath = "../../../Instancias/Vrp-Set-A/A-n249-m5-Q500.vrp";

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

    float max_dist_between_nodes = 200;
    int max_number_vehicles = 10;

    auto calc = [&](Individual x)
    {
        auto [demanda_atendida, dist_percorrida] = splitting_chromossome(dist_nodes_mat, dist_depots_nodes_mat, x, demand,
                                                                         CAPACITY, max_dist_between_nodes, max_number_vehicles);

        return std::vector<float>({dist_percorrida, -demanda_atendida});
    };

    int number_of_obj = 2;

    auto [pop, pop_obj_val] = NSGAII_mod(100, 100, demand.size(), calc, number_of_obj, 0.15, initialize_population);

    print_solution_csv(pop, pop_obj_val);
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

std::tuple<std::vector<Individual>, std::vector<std::vector<float>>> NSGAII_mod(int size_pop, int number_generations, int number_cities, auto f, int number_obj, float mutation_rate, auto initialize_pop_func)
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

    return {parent_pop, parent_pop_obj_val};
}

// TODO pop should be related to the problem
// TODO the crossover function should be specific to problem (could be a input of function)
std::vector<Individual> recombination(std::vector<int> &parents, std::vector<Individual> &pop, std::uniform_real_distribution<> &unif)
{
    int size = pop[0].size();
    auto N = pop.size();

    std::vector<Individual> next_gen;
    next_gen.reserve(parents.size());

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
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

// TODO specific to the problem
void mutate(Individual &child, const float &mutation_rate, std::default_random_engine &engine)
{
    std::uniform_real_distribution<> mutate_coin(0, 1);

    if (mutate_coin(engine) < mutation_rate)
    {
        // https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
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