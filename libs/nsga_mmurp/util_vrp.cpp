#include "util_vrp.h"
#include <set>

float open_route_distance(std::vector<std::vector<float>>& dist, std::vector<int>& route)
{
    size_t route_size = route.size();
    float total_dist = 0;

    for (size_t i = 0; i < route_size - 1; i++)
    {
        total_dist += dist[route[i]][route[i + 1]];
    }

    return total_dist;
}

// std::vector<int> 

std::vector<int> crossover_OX(std::vector<int> &P1, std::vector<int> &P2, int cut_begin, int cut_end)
{
    int size = P1.size();

    std::vector<int> child(P1);

    std::set<int> cut_itens(&(P1[cut_begin]), &(P1[cut_end+1]));

    int child_k = (cut_end + 1) % size;

    for (int k_ = 0; k_ < size; k_++)
    {
        int k = (k_ + cut_end + 1) % size;
        if (cut_itens.find(P2[k]) == cut_itens.end()) // não encontrou
        {
            child[child_k] = P2[k];
            // std::cout << "BEFORE INC: " << child_k << std::endl;
            child_k = (child_k + 1) % size;
            // std::cout << "AFTER INC: " << child_k << std::endl;
        }
    }

    return child;
}