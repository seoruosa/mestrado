#include <iostream>
#include <sstream>
#include <algorithm> //sort
#include <limits>    //numeric_limits
#include <filesystem>

#include "nsgaii.cpp"
#include "nsgaii_mmurp.h"
#include "../splitting/mommurp.h"
#include "permutation.h"

#include "util_print.h"
#include "util_vrp.h"

#include "../read_instance/mmurp.h"
#include "../utils/performance.h"
#include "../read_instance/mmurp_data.h"

int main(const int argc, const char *argv[])
{
    NSGAII_MMURP_Params CONFIG = read_input(argc, argv);
    std::string name = std::filesystem::path(CONFIG.instance_path).stem().string();

    MMURP_data instance_data(CONFIG.instance_path);

    auto REF_POINT = ref_point(instance_data.dist_nodes_nodes, instance_data.dist_depots_nodes);

    std::ofstream BEGIN_OUT_FILE(file_name(name, "in"), std::ios::ate);
    std::ofstream END_OUT_FILE(file_name(name, "out"), std::ios::ate);

    print_params(CONFIG, instance_data.vehicle_capacity, name, BEGIN_OUT_FILE);
    BEGIN_OUT_FILE << REF_POINT << std::endl;
    print_params(CONFIG, instance_data.vehicle_capacity, name, END_OUT_FILE);
    END_OUT_FILE << REF_POINT << std::endl;

    auto calc = [&](Individual x)
    {
        SplittedResult splitting_result = splitting2(instance_data.dist_nodes_nodes, instance_data.dist_depots_nodes, x, instance_data.demand, instance_data.number_vehicles,
                                                    instance_data.vehicle_capacity, instance_data.max_travel_dist);
        
        auto [dist_percorrida, demanda_atendida] = bestResult(splitting_result.lambda);

        return std::vector<float>({dist_percorrida, -demanda_atendida});
    };

    int number_of_obj = 2;

    auto [pop, pop_obj_val] = NSGAII_mod(CONFIG.size_pop, CONFIG.number_generations * instance_data.n_clients, instance_data.n_clients, calc, number_of_obj,
                                         CONFIG.mutation_rate, initialize_population, BEGIN_OUT_FILE, END_OUT_FILE);

    std::cout << REF_POINT << std::endl;
    print_obj_val(pop_obj_val);

    BEGIN_OUT_FILE.close();
    END_OUT_FILE.close();
}

std::vector<Individual> initialize_population(int size_pop, int n_cities)
{
    unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
    auto RND_ENGINE = std::default_random_engine(SEED);

    std::vector<Individual> pop(size_pop, generate_int_values(n_cities));

    for (auto &ind : pop)
    {
        std::shuffle(ind.begin(), ind.end(), RND_ENGINE);
    }

    return pop;
}

std::tuple<std::vector<Individual>, std::vector<std::vector<float>>> NSGAII_mod(int size_pop, int number_generations, int number_cities,
                                                                                auto f, int number_obj, float mutation_rate, auto initialize_pop_func, std::ostream &BEGIN_FILE, std::ostream &END_FILE)
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
    std::vector<float> distances;
    std::vector<int> perm;

    // initialize population
    // TODO need to create a initialization for MMURP
    // TODO parent_pop will be necessary to change type
    std::vector<Individual> parent_pop(initialize_pop_func(size_pop, number_cities));

    // TODO parent_pop_obj_val change type
    // TODO evaluate_pop_obj need to accept different parent_pop input type (prob not, because it's using auto...)
    std::vector<std::vector<float>> parent_pop_obj_val(evaluate_pop_obj(parent_pop, f, number_obj));

    print_solution_csv(parent_pop, parent_pop_obj_val, BEGIN_FILE);
    // print_mat(parent_pop);
    // std::cout << "*****************************************" << std::endl;
    // print_mat(parent_pop_obj_val);
    for (int generation = 0; generation < number_generations; generation++)
    {
        perf::save_gen(generation, MEASUREMENT_FILE);

        start_gen = perf::time::start();
        time_start = perf::time::start();
        // first child generation
        // TODO child_pop need to change the type
        // TODO should be a function input
        child_pop = generate_next_generation(parent_pop_obj_val, parent_pop, mutation_rate);
        time_duration = perf::time::duration(time_start);
        
        perf::save_measurement("next_generation:generate", time_duration, MEASUREMENT_FILE);

        // print_mat(child_pop);
        // return;
        // std::vector<Individual> child_pop = generate_next_gen(parent_pop_obj_val, parent_pop, mutation_rate);
        time_start = perf::time::start();
        child_pop_obj_val = evaluate_pop_obj(child_pop, f, number_obj);

        next_gen_pop = child_pop;
        next_gen_pop_obj_val = child_pop_obj_val;
        
        time_duration = perf::time::duration(time_start);
        perf::save_measurement("next_generation:evaluate", time_duration, MEASUREMENT_FILE);

        // TODO make generations an object to be easier to make the transition from one gen to other
        // create the next generation
        time_start = perf::time::start();

        next_gen_pop.insert(next_gen_pop.end(), parent_pop.begin(), parent_pop.end());
        next_gen_pop_obj_val.insert(next_gen_pop_obj_val.end(), parent_pop_obj_val.begin(), parent_pop_obj_val.end());
        
        time_duration = perf::time::duration(time_start);
        perf::save_measurement("next_generation:insert", time_duration, MEASUREMENT_FILE);

        time_start = perf::time::start();
        auto [fronts, rank] = fast_non_dominated_sort(next_gen_pop_obj_val);
        time_duration = perf::time::duration(time_start);
        perf::save_measurement("next_generation:fast_sort", time_duration, MEASUREMENT_FILE);

        time_start = perf::time::start();
        std::vector<std::vector<int>> next_pop;
        std::vector<int> next_pop_idx;
        int i = 0;
        while ((int)(next_pop_idx.size() + fronts[i].size()) <= size_pop)
        {
            next_pop_idx.insert(next_pop_idx.begin(), fronts[i].begin(), fronts[i].end());
            ++i;
        }
        time_duration = perf::time::duration(time_start);
        perf::save_measurement("next_generation:select_except_last", time_duration, MEASUREMENT_FILE);

        time_start = perf::time::start(); // start next_generation:crowd_sort_and_finish
        distances = crowding_distance_assignment(next_gen_pop_obj_val, fronts[i]);

        perm = generate_int_values(fronts[i].size());
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
            parent_pop_obj_val[i] = next_gen_pop_obj_val[idx];
        }
        // parent_pop_obj_val = evaluate_pop_obj(parent_pop, f, number_obj);
        time_duration = perf::time::duration(time_start);
        perf::save_measurement("next_generation:crowd_sort_and_finish", time_duration, MEASUREMENT_FILE);
    }

    time_duration = perf::time::duration(start);
    perf::save_measurement("nsga:total", time_duration, MEASUREMENT_FILE);

    END_FILE << "DURACAO : " << (time_duration.count()/1.e6) << std::endl;

    print_solution_csv(parent_pop, parent_pop_obj_val, END_FILE);

    MEASUREMENT_FILE.close();
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

NSGAII_MMURP_Params read_input(const int &argc, const char *argv[])
{
    std::vector<std::string> arguments(argv + 1, argv + argc);

    std::string instance_path = arguments[0];
    // float max_dist_between_nodes;
    // int max_number_vehicles;
    int size_pop;
    int number_generations;
    float mutation_rate;

    for (int i = 1; i < (int)arguments.size(); i += 2)
    {
        auto arg_name = arguments[i];
        auto arg_value = arguments[i + 1];

        if (arg_name == "--sizePop")
        {
            size_pop = std::stoi(arg_value);
        }
        else if (arg_name == "--numGen")
        {
            number_generations = std::stoi(arg_value);
        }
        else if (arg_name == "--mutationRate")
        {
            mutation_rate = std::stof(arg_value);
        }
    }

    return {instance_path, size_pop, number_generations, mutation_rate};
}

void print_params(NSGAII_MMURP_Params config, int capacity, const std::string &name, std::ostream &out)
{
    out << "NAME: " << name << std::endl;
    out << "MUTATION RATE: " << config.mutation_rate << std::endl;
    out << "NUMBER GENERATIONS: " << config.number_generations << std::endl;
    out << "POPULATION SIZE: " << config.size_pop << std::endl;
    out << "-----------------END_PARAMS-----------------" << std::endl;
}

std::string instance_name(std::string const &begin, int n, int m, int Q, int max_veh)
{
    std::ostringstream name;

    name << begin << "-n" << n << "-m" << m << "-Q" << Q;

    if (max_veh < std::numeric_limits<int>::max())
    {
        name << "-v" << max_veh;
    }

    return name.str();
}

std::string instance_name(std::string const &begin, int n, int m, int Q, const std::vector<int> &max_veh)
{
    std::ostringstream name;

    name << begin << "-n" << n << "-m" << m << "-Q" << Q << "-v";

    for (int i = 0; i < m - 1; i++)
    {
        name << max_veh.at(i) << "_";
    }
    name << max_veh.back();
    

    return name.str();
}

std::string file_name(std::string const &begin, int n, int m, int Q, int max_veh, std::string const &extension)
{
    std::ostringstream name;

    name << instance_name(begin, n, m, Q, max_veh);

    name << "-" << time_now("%Y%m%d%H%M%S") << "." << extension;

    return name.str();
}

std::string file_name(std::string const &begin, std::string const &extension)
{
    std::ostringstream name;

    name << begin << "-" << time_now("%Y%m%d%H%M%S") << "." << extension;

    return name.str();
}

float max_dist(std::vector<std::vector<float>> &dist_nodes_mat, std::vector<std::vector<float>> &dist_depots_nodes_mat)
{
    float dist = 0;

    for (auto &row : dist_nodes_mat)
    {
        float a = *std::max_element(row.begin(), row.end());

        dist = std::max(dist, a);
    }

    for (auto &row : dist_depots_nodes_mat)
    {
        float a = *std::max_element(row.begin(), row.end());

        dist = std::max(dist, a);
    }

    return dist;
}

std::string ref_point(std::vector<std::vector<float>> &dist_nodes_mat, std::vector<std::vector<float>> &dist_depots_nodes_mat)
{
    std::ostringstream name;

    float max_travel_dist = 1.1 * max_dist(dist_nodes_mat, dist_depots_nodes_mat) * dist_nodes_mat.size();

    name << "REF_POINT: " << max_travel_dist << ", " << 0;

    return name.str();
}