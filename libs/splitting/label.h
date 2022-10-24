#pragma once
#include <tuple>
#include <vector>
#include <list>

typedef std::tuple<float, float> DistDemand;

class Label
{
private:
    float demand_;
    float distance_;
    std::vector<int> used_trucks_;
    int num_depot_;

    static size_t instances_counter_;
    size_t counter_;
public:
    // Label(float demand, float distance, std::vector<int> &used_trucks);
    Label(int num_depot);
    Label(const Label& label_);
    // ~Label();
    DistDemand distDemand();
    std::vector<int> &used_trucks();
    float distance();
    float demand();
    
    Label& inc_demand(float inc);
    Label& inc_distance(float inc);
    Label& inc_num_vehicles_depot(int depot);

    int num_vehicles_depot(int depot);

    bool weak_dominates(Label& second);
    bool weak_is_dominated(Label& second);

    bool is_not_dominated_by(std::list<Label>& other_labels);

    size_t get_index();

    friend std::ostream& operator<< (std::ostream& os, const Label& label);
};

template <typename T>
bool weak_dominates_vec(std::vector<T>& u, std::vector<T>& v);

template <typename T>
bool weak_is_dominated_vec(std::vector<T>& u, std::vector<T>& v);