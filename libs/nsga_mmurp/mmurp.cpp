#include "mmurp.h"
#include <limits>

// return {demanda_atendida, dist_percorrida, trip_depot, trips};
// std::tuple<float, float, std::vector<int>, std::vector<std::vector<int>>> splitting_chromossome(std::vector<std::vector<float>> &dist_nodes_demand,
std::tuple<float, float> splitting_chromossome(std::vector<std::vector<float>> &dist_nodes_demand,
                                               std::vector<std::vector<float>> dist_nodes_depot,
                                               std::vector<int> &chromossome,
                                               std::vector<float> &demanda,
                                               float max_load,
                                               float max_dist_between_nodes,
                                               int max_number_vehicles)
{
    /*
    Prins, Christian. "A simple and effective evolutionary algorithm for the
    vehicle routing problem." Computers & operations research 31.12 (2004): 1985-2002.
    */

    /*
         * Como limitar o número de MMUs?
            * selecionar M primeiros resultados [ISSO AQUI Q ESTÀ IMPLEMENTADO]
            * resolver o problema da mochila multiobjetivo que é encontrar o maior atendimento de demanda com menor distancia percorrida
         * O objetivo não é somente minimizar o custo, mas também maximizar
             o número de pessoas atendidas. Como fazer? problema da mochila?
    */
    int n = chromossome.size();

    // V_j cost of the shortest path from node 0 to node j
    std::vector<float> V(n + 1, std::numeric_limits<float>::max());
    std::vector<int> precedent(n + 1, 0);
    std::vector<int> depot_of_vehicle(n + 1, 0); // depot of each node
    V[0] = 0;

    // function that return index of chromossome index
    auto S = [chromossome](int i)
    { return chromossome[i - 1]; };

    for (int i = 1; i <= n + 1; i++)
    {
        float load = 0;
        float cost = 0;
        int j = i;
        int depot = 0;
        bool is_dist_greater_max = false;

        while (j <= n && load <= max_load && !is_dist_greater_max)
        {
            load = load + demanda[S(j)];

            if (i == j)
            {
                cost = std::numeric_limits<float>::max();

                for (int m = 0; m < dist_nodes_depot.size(); m++)
                {
                    auto actual_cost = dist_nodes_depot[m][S(j)];

                    if (actual_cost < cost)
                    {
                        depot = m;
                        cost = actual_cost;
                    }
                }
            }
            else
            {
                float dist_between_nodes = dist_nodes_demand[S(j - 1)][S(j)];
                is_dist_greater_max = dist_between_nodes > max_dist_between_nodes;

                cost = cost + dist_between_nodes;
            }

            if (load <= max_load && !is_dist_greater_max)
            {
                // talvez seja aqui que no lugar de realizar esta comparação, façamos uma comparação de dominância
                if (V[i - 1] + cost < V[j])
                {
                    V[j] = V[i - 1] + cost;
                    precedent[j] = i - 1;
                    depot_of_vehicle[j] = depot;
                }
                j = j + 1;
            }
        }
    }

    // TODO separar em duas funções
    std::vector<std::vector<int>> trips;
    std::vector<int> trip_depot;

    int j = n;
    int i = n;

    while (i != 0)
    {
        // trips.push_back({depot_of_vehicle[j]});
        trip_depot.push_back(depot_of_vehicle[j]);
        trips.push_back({});
        i = precedent[j];
        for (int k = i + 1; k <= j; k++)
        {
            trips[trips.size() - 1].push_back(S(k));
        }
        j = i;
    }

    // TODO não deve ser necessário definir as trips e depois fazer os cálculos
    // TODO calcular o ótimo com um número limitado de veículos (agora está calculando um subótimo)
    float demanda_atendida = 0;
    float dist_percorrida = 0;

    for (int i_trip = 0; i_trip < trips.size() && i_trip < max_number_vehicles; i_trip++)
    {
        demanda_atendida = demanda_atendida + demanda[trips[i_trip][0]];
        dist_percorrida = dist_percorrida + dist_nodes_depot[trip_depot[i_trip]][trips[i_trip][0]];

        for (int i = 1; i < trips[i_trip].size(); i++)
        {
            demanda_atendida = demanda_atendida + demanda[trips[i_trip][i]];
            dist_percorrida = dist_percorrida + dist_nodes_demand[trips[i_trip][i - 1]][trips[i_trip][i]];
        }
    }

    // return {demanda_atendida, dist_percorrida, trip_depot, trips};
    return {demanda_atendida, dist_percorrida};
}