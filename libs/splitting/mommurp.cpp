#include "label.h"
#include "mommurp.h"
// #include "util.h"

#include <limits>
#include <iostream>
#include <string>
#include <numeric>
#include <algorithm>

SplittedResult splitting(const std::vector<std::vector<float>> &dist_nodes_demand,
                         const std::vector<std::vector<float>> &dist_nodes_depot,
                         const std::vector<int> &big_tour,
                         const std::vector<float> &demand,
                         const std::vector<int> &max_number_vehicles,
                         const float &max_demand,
                         const float &max_dist_between_nodes)
{
    int n = big_tour.size();
    int number_depots = max_number_vehicles.size();

    Lambda lambda(n + 1, std::list<Label>({}));
    MapPred map_label_pred;

    lambda[0].push_back(Label(number_depots));    

    // function that return index of big tour index i
    auto T = [big_tour](int i)
    { return big_tour[i - 1]; };

    auto remove_all_dominated_by = [](std::list<Label> &labels_to_clean, Label &next_label)
    {
        labels_to_clean.remove_if([&](auto el)
                                  { return (next_label.weak_dominates(el)); });
    };

    for (int i = 1; i <= n; i++)
    {
        for (int dk = 0; dk < number_depots; dk++)
        {

            for (auto &label : lambda[i - 1])
            {
                int j = i;
                float tour_demand = 0;
                float tour_distance = 0;

                bool stop = false;
                Label next_label(label);

                do
                {
                    tour_demand += demand[T(j)];

                    if (i == j)
                    {
                        if (label.num_vehicles_depot(dk) + 1 <= max_number_vehicles[dk])
                        {
                            tour_distance = dist_nodes_depot[dk][T(i)];

                            next_label.inc_distance(tour_distance);
                            next_label.inc_demand(demand[T(i)]);
                            next_label.inc_num_vehicles_depot(dk);

                            if (next_label.is_not_dominated_by(lambda[i]))
                            {
                                remove_all_dominated_by(lambda[i], next_label);
                                lambda[i].push_back(next_label);

                                map_label_pred[lambda[i].back().get_index()] = {label.get_index(), dk};
                            }
                        }
                        else
                        {
                            stop = true;
                        }
                    }
                    else
                    {
                        float dist_prev_actual_node = dist_nodes_demand[T(j - 1)][T(j)];

                        if ((dist_prev_actual_node <= max_dist_between_nodes) &&
                            (tour_demand <= max_demand))
                        {
                            stop = false;

                            tour_distance = dist_prev_actual_node;

                            next_label.inc_distance(tour_distance);
                            next_label.inc_demand(demand[T(j)]);

                            if (next_label.is_not_dominated_by(lambda[j]))
                            {
                                remove_all_dominated_by(lambda[j], next_label);
                                lambda[j].push_back(next_label);

                                map_label_pred[lambda[j].back().get_index()] = {label.get_index(), dk};
                            }
                        }
                        else
                        {
                            stop = true;
                        }
                    }

                    ++j;
                } while ((j < n) && (!stop));
            }
        }
    }

    return {lambda, map_label_pred};
}

SplittedResult splitting2(const std::vector<std::vector<float>> &dist_nodes_demand,
                         const std::vector<std::vector<float>> &dist_nodes_depot,
                         const std::vector<int> &big_tour,
                         const std::vector<float> &demand,
                         const std::vector<int> &max_number_vehicles,
                         const float &max_demand,
                         const float &max_dist_between_nodes)
{
    int n = big_tour.size();
    int number_depots = max_number_vehicles.size();

    Lambda lambda(n + 1, std::list<Label>({}));
    MapPred map_label_pred;

    lambda[0].push_back(Label(number_depots));    

    // function that return index of big tour index i
    auto T = [big_tour](int i)
    { return big_tour[i - 1]; };

    auto remove_all_dominated_by = [](std::list<Label> &labels_to_clean, Label &next_label)
    {
        labels_to_clean.remove_if([&](auto el)
                                  { return (next_label.weak_dominates(el)); });
    };

    for (int i = 1; i <= n; i++)
    {
        for (int dk = 0; dk < number_depots; dk++)
        {

            for (auto &label : lambda[i - 1])
            {
                int j = i;
                float tour_demand = 0;
                float tour_distance = 0;

                bool stop = false;
                Label next_label(label);

                do
                {
                    tour_demand += demand[T(j)];

                    if (i == j)
                    {
                        if (label.num_vehicles_depot(dk) + 1 <= max_number_vehicles[dk])
                        {
                            tour_distance = dist_nodes_depot[dk][T(i)];

                            next_label.inc_distance(tour_distance);
                            next_label.inc_demand(demand[T(i)]);
                            next_label.inc_num_vehicles_depot(dk);

                            if (next_label.is_not_dominated_by(lambda[i]))
                            {
                                remove_all_dominated_by(lambda[i], next_label);
                                lambda[i].push_back(next_label);

                                map_label_pred[lambda[i].back().get_index()] = {label.get_index(), dk};
                            }
                        }
                        else
                        {
                            stop = true;
                        }
                    }
                    else
                    {
                        float dist_prev_actual_node = dist_nodes_demand[T(j - 1)][T(j)];

                        if ((dist_prev_actual_node <= max_dist_between_nodes) &&
                            (tour_demand <= max_demand))
                        {
                            stop = false;

                            tour_distance = dist_prev_actual_node;

                            next_label.inc_distance(tour_distance);
                            next_label.inc_demand(demand[T(j)]);

                            if (next_label.is_not_dominated_by(lambda[j]))
                            {
                                remove_all_dominated_by(lambda[j], next_label);
                                lambda[j].push_back(next_label);

                                map_label_pred[lambda[j].back().get_index()] = {label.get_index(), dk};
                            }
                        }
                        else
                        {
                            stop = true;
                        }
                    }

                    ++j;
                } while ((j < n) && (!stop));
            }
        }
    }

    return {lambda, map_label_pred};
}

DistDemand bestResult(const Lambda &result)
{
    struct
    {
        bool operator()(Label a, Label b) const
        {
            return (a.demand() >= b.demand() && a.distance() < b.distance());
        }
    } customLess;

    for (auto el = result.rbegin(); el != result.rend(); el++)
    {
        if (!(*el).empty())
        {
            auto min = std::min_element((*el).begin(), (*el).end(), customLess);
            Label output(*min);
            return output.distDemand();
        }
    }
    
    return Label(0).distDemand();
}

void print_map_pred(std::string_view comment, const std::map<size_t, PredDepot> &m)
{
    std::cout << "------------------------------------" << std::endl;
    std::cout << comment << std::endl;
    // iterate using C++17 facilities
    for (const auto &[key, value] : m)
    {
        std::cout << '[' << key << "] = (" << value.pred << ", " << value.depot << "); ";
        std::cout << '\n';
    }
    std::cout << "------------------------------------" << std::endl;
}

/*
// Version with Logging
void Log()
{
    std::cout << "DEBUG >> ";
}

template <typename T>
void Log(std::string const &message, T val)
{
    Log();
    std::cout << message << " : " << val << std::endl;
}

void Log(std::string const &message)
{
    Log();
    std::cout << message << std::endl;
}

SplittedResult splitting(const std::vector<std::vector<float>> &dist_nodes_demand,
                        const std::vector<std::vector<float>> &dist_nodes_depot,
                        const std::vector<int> &big_tour,
                        const std::vector<float> &demand,
                        const std::vector<int> &max_number_vehicles,
                        const float &max_demand,
                        const float &max_dist_between_nodes)
{
    int n = big_tour.size();
    int number_depots = max_number_vehicles.size();

    Lambda lambda(n + 1, std::list<Label>({}));
    MapPred map_label_pred;

    lambda[0].push_back(Label(number_depots));

    // function that return index of big tour index i
    auto T = [big_tour](int i)
    { return big_tour[i - 1]; };

    auto remove_all_dominated_by = [](std::list<Label> &labels_to_clean, Label &next_label)
    {
        labels_to_clean.remove_if([&](auto el)
                                  { return (next_label.weak_dominates(el)); });
    };

    print_linha(2);
    Log("Parâmetros");
    print_linha(2);

    Log("Tamanho tour", n);
    Log("Demanda total", std::accumulate(demand.begin(), demand.end(), 0));

    print_linha(2);
    Log("Comecou!!!");
    print_linha(2);

    for (int i = 1; i <= n; i++)
    {
        for (int dk = 0; dk < number_depots; dk++)
        {
            Log("max_number_vehicles[dk]", max_number_vehicles[dk]);
            Log("dk", dk);
            print_linha();

            for (auto &label : lambda[i - 1])
            {
                int j = i;
                float tour_demand = 0;
                float tour_distance = 0;

                print_linha();
                print_lambda(lambda);

                Log(" >>>>>     label to be iterated", label);
                bool stop = false;
                Label next_label(label);
                do
                {
                    tour_demand += demand[T(j)];

                    Log("dk", dk);
                    Log("i", i);
                    Log("j", j);
                    Log("T(j)", T(j));
                    Log("Demand actual city", demand[T(j)]);
                    Log("tour_demand", tour_demand);

                    if (i == j)
                    {
                        Log("i==j >>>>>     next_label", next_label);

                        if (label.num_vehicles_depot(dk) + 1 <= max_number_vehicles[dk])
                        {
                            tour_distance = dist_nodes_depot[dk][T(i)];

                            next_label.inc_distance(tour_distance);
                            next_label.inc_demand(demand[T(i)]);
                            next_label.inc_num_vehicles_depot(dk);

                            if (next_label.is_not_dominated_by(lambda[i]))
                            {
                                Log("Not Dominated (starting tour)", next_label);

                                remove_all_dominated_by(lambda[i], next_label);
                                lambda[i].push_back(next_label);

                                map_label_pred[lambda[i].back().get_index()] = {label.get_index(), dk};

                                print_linha();
                                print_map_pred("Map starting a new trip", map_label_pred);
                                print_linha();
                                print_lambda(lambda);
                            }
                            else
                            {
                                Log("Dominated (starting tour)", next_label);
                                print_lambda(lambda);
                            }
                        }
                        else
                        {
                            stop = true;
                            Log("Its not possible to start a new trip on this depot");
                        }
                    }
                    else
                    {
                        Log("i!=j >>>>>     next_label", next_label);

                        float dist_prev_actual_node = dist_nodes_demand[T(j - 1)][T(j)];

                        Log("Testing if it's possible to attend city", T(j));
                        Log("dist_prev_actual_node", dist_prev_actual_node);
                        Log("tour_demand", tour_demand);

                        if ((dist_prev_actual_node <= max_dist_between_nodes) &&
                            (tour_demand <= max_demand))
                        {
                            stop = false;
                            Log("It's possible to attend a new city");

                            tour_distance = dist_prev_actual_node;

                            next_label.inc_distance(tour_distance);
                            next_label.inc_demand(demand[T(j)]);

                            if (next_label.is_not_dominated_by(lambda[j]))
                            {
                                Log("Not dominated. next_label", next_label);

                                remove_all_dominated_by(lambda[j], next_label);
                                lambda[j].push_back(next_label);

                                map_label_pred[lambda[j].back().get_index()] = {label.get_index(), dk};
                                print_map_pred("Map continue some previous trip", map_label_pred);
                                print_lambda(lambda);
                            }
                            else
                            {
                                Log("next_label is dominated", next_label);
                            }
                        }
                        else
                        {
                            stop = true;
                        }
                        Log("gera próximo. stop", (stop ? "true" : "false"));
                    }

                    ++j;

                    Log(">>>>>>>>>>>>> j", j);
                } while ((j < n) && (!stop));

                Log("Saiu do do/while. j", j);
                Log("n", n);
                Log("stop", (stop ? "true" : "false"));
            }
        }
    }

    print_linha(2);
    print_map_pred("Map com predecessores", map_label_pred);
    std::cout << map_label_pred.size() << std::endl;
    print_linha(2);

    return {lambda, map_label_pred};
}
*/