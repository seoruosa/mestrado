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

    int j;
    float tour_demand;
    float tour_distance;
    bool stop;

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
                j = i;
                tour_demand = 0;
                tour_distance = 0;

                stop = false;
                Label next_label(label);

                while ((j < n) && (!stop))
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
                }
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