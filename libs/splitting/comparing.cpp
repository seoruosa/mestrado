#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <chrono>
#include <algorithm>


#include "tests/util.h"
#include "util.h"

#include "../read_instance/mmurp.h"
#include "mommurp.h"

int main(const int argc, const char *argv[])
{
    std::cout << "COMPARE ALGORITHMS" << std::endl;
    std::vector<std::string> arguments(argv + 1, argv + argc);

    std::string filepath = arguments[0];

    int repetitions = 100;

    if (arguments.size() > 1)
    {
        repetitions = std::stoi(arguments[1]);
    }

    int CAPACITY; // TODO should be float
    std::vector<std::vector<float>> dist_nodes_mat;
    std::vector<std::vector<float>> dist_depots_nodes_mat;
    std::vector<float> demanda;
    std::vector<int> max_number_vehicles;
    float max_travel_dist;

    std::string name = "playground";

    read_instance(dist_nodes_mat, dist_depots_nodes_mat, demanda, CAPACITY, max_number_vehicles, max_travel_dist,
                  name, filepath);

    auto print_vetor = [](auto vetor)
    {
    for (int i = 0; i < vetor.size() - 1; i++)
    {
        std::cout << vetor[i] << " | ";
    }
    std::cout << vetor.back() << std::endl; };

    int n = dist_nodes_mat.size();
    std::vector<int> big_tour(n, 0);
    for (int i = 0; i < n; i++)
    {
        big_tour[i] = i;
    }

    std::cout << "size: " << n << std::endl;
    std::cout << "capacidade: " << CAPACITY << std::endl;
    std::cout << "big_tour size: " << big_tour.size() << std::endl;
    std::cout << "max_travel_dist: " << max_travel_dist << std::endl;
    std::cout << "dist_nodes_mat_cols: " << dist_nodes_mat[0].size() << std::endl;
    std::cout << "depots: " << max_number_vehicles.size() << std::endl;
    std::cout << "demanda.size: " << demanda.size() << std::endl;
    std::cout << "caminhoes por depot: ";
    print_vetor(max_number_vehicles);
    

    std::cout << filepath << std::endl;
    std::cout << "*************************" << std::endl;

    // unsigned SEED = 893;
    unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
    auto RND_ENGINE = std::default_random_engine(SEED);

    std::cout << "SEED: " << SEED << std::endl;
    std::cout << "REPETITIONS: " << repetitions << std::endl;
    bool all_results_equal = true;

    for (int i = 0; i < repetitions; ++i)
    {
        if (i > 0)
        {
            std::shuffle(big_tour.begin(), big_tour.end(), RND_ENGINE);
        }

        // SPLITTING 1
        auto start = std::chrono::high_resolution_clock::now();

        auto [result, map] = splitting(dist_nodes_mat, dist_depots_nodes_mat, big_tour,
                                       demanda, max_number_vehicles, CAPACITY, max_travel_dist);
        auto end_split_1 = std::chrono::high_resolution_clock::now();

        auto [best_dist, best_demand] = bestResult(result);

        auto end_total_1 = std::chrono::high_resolution_clock::now();        

        // SPLITTING 2

        auto [result2, map2] = splitting(dist_nodes_mat, dist_depots_nodes_mat, big_tour,
                                       demanda, max_number_vehicles, CAPACITY, max_travel_dist);

        auto end_split_2 = std::chrono::high_resolution_clock::now();
        

        auto [best_dist_2, best_demand_2] = bestResult(result);

        auto end_total_2 = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> duration_split_1(end_split_1 - start);
        std::chrono::duration<double, std::micro> duration_total_1(end_total_1 - start);

        std::chrono::duration<double, std::micro> duration_split_2(end_split_2 - end_total_1);
        std::chrono::duration<double, std::micro> duration_total_2(end_total_2 - end_total_1);
        bool compare_results = lambdas_are_equal(result2, result);

        std::cout << "RESULTADOS >> " << (compare_results ? "IGUAIS" : "DIFERENTES") << std::endl;
        std::cout << "DURACAO SPLITTING 1: " << duration_split_1.count() / 1E6 << std::endl;
        std::cout << "DURACAO TOTAL 1: " << duration_total_1.count() / 1E6 << std::endl;
        std::cout << "DURACAO SPLITTING 2: " << duration_split_2.count() / 1E6 << std::endl;
        std::cout << "DURACAO TOTAL 2: " << duration_total_2.count() / 1E6 << std::endl;

        std::cout << "dist: " << best_dist << ", demand: " << best_demand << std::endl;
        std::cout << ">> " << i << " *************************" << std::endl;

        if (!compare_results)
        {
            all_results_equal = false;
        }
    }

    std::cout << std::endl;
    std::cout << "*************************" << std::endl;
    std::cout << "**********ACABOU*********" << std::endl;
    std::cout << "*************************" << std::endl;
    std::cout << "**********" << (all_results_equal ? "TODOS IGUAIS" : "ALGUM DIFERENTE") << "**********" << std::endl;
    std::cout << "*************************" << std::endl;
}