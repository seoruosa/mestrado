#pragma once

#include "label.h"
#include <vector>
#include <map>

#include "../read_instance/mmurp_data.h"

typedef struct
{
    size_t pred;
    int depot;
} PredDepot;

// this structure could be changed to a improved one, to be easily the removal of elements
typedef std::vector<std::list<Label>> Lambda;
typedef std::map<size_t, PredDepot> MapPred;

typedef struct
{
    Lambda lambda;
    MapPred pred;
} SplittedResult;

void print_map_pred(std::string_view comment, const std::map<size_t, PredDepot>& m);

SplittedResult splitting(const std::vector<std::vector<float>> &dist_nodes_demand,
                        const std::vector<std::vector<float>> &dist_nodes_depot,
                        const std::vector<int> &big_tour,
                        const std::vector<float> &demand,
                        const std::vector<int> &max_number_vehicles,
                        const float &max_demand,
                        const float &max_dist_between_nodes);

SplittedResult splitting2(const std::vector<std::vector<float>> &dist_nodes_demand,
                         const std::vector<std::vector<float>> &dist_nodes_depot,
                         const std::vector<int> &big_tour,
                         const std::vector<float> &demand,
                         const std::vector<int> &max_number_vehicles,
                         const float &max_demand,
                         const float &max_dist_between_nodes);

void print_lambda(const Lambda &lambda);
DistDemand bestResult(const Lambda &result);

class Split_mmurp
{
protected:
    MMURP_data instance;
public:
    Split_mmurp(MMURP_data &instance);
    ~Split_mmurp();
    SplittedResult solve(const std::vector<int> &big_tour);
};