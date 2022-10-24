#include "label.h"
#include <iostream>

// Label::Label(float demand, float distance, std::vector<int> &used_trucks)
// {
//     demand_ = demand;
//     distance_ = distance;
//     used_trucks_ = std::vector<int> (used_trucks);
// }

size_t Label::instances_counter_ = 0;

Label::Label(int num_depot)
{
    // std::cout << ">> BUILDING Label: #depot: " << num_depot << std::endl;
    demand_ = 0;
    distance_ = 0;
    used_trucks_ = std::vector<int>(num_depot, 0);
    num_depot_ = num_depot;

    ++instances_counter_;
    counter_ = instances_counter_;
}
Label::Label(const Label &label)
{
    // std::cout << ">> COPYING Label: " << label << std::endl;
    demand_ = label.demand_;
    distance_ = label.distance_;
    used_trucks_ = std::vector<int>(label.used_trucks_.begin(), label.used_trucks_.end());
    num_depot_ = label.num_depot_;

    ++instances_counter_;
    counter_ = instances_counter_;
}

// Label::~Label()
// {
// }

DistDemand Label::distDemand()
{
    return DistDemand({distance_, demand_});
}

std::vector<int> &Label::used_trucks()
{
    return used_trucks_;
}

float Label::distance()
{
    return distance_;
}

float Label::demand()
{
    return demand_;
}

Label &Label::inc_demand(float inc)
{
    demand_ += inc;

    return *this;
}

Label &Label::inc_distance(float inc)
{
    distance_ += inc;

    return *this;
}

Label &Label::inc_num_vehicles_depot(int depot)
{
    used_trucks_.at(depot)++;

    return *this;
}

int Label::num_vehicles_depot(int depot)
{
    return used_trucks_.at(depot);
}

bool Label::weak_dominates(Label &second)
{
    return ((demand_ >= second.demand_) &&
            (distance_ <= second.distance_) &&
            weak_dominates_vec(used_trucks_, second.used_trucks_));
}

bool Label::weak_is_dominated(Label &second)
{
    return ((second.demand_ >= demand_) &&
            (second.distance_ <= distance_) &&
            weak_dominates_vec(second.used_trucks_, used_trucks_));
}

bool Label::is_not_dominated_by(std::list<Label> &other_labels)
{
    for (auto &label : other_labels)
    {
        bool is_dominated = weak_is_dominated(label);

        if (is_dominated)
        {
            return false;
        }
    }
    return true;
}

size_t Label::get_index()
{
    return counter_;
}

std::ostream &
operator<<(std::ostream &os, const Label &label)
{
    os << '#' << label.counter_ << ' ';

    os << '(' << label.distance_ << ',';
    os << label.demand_ << '|';

    for (int i = 0; i < label.num_depot_; i++)
    {
        os << label.used_trucks_[i];
        if (i < label.num_depot_ - 1)
        {
            os << ',';
        }
    }

    os << ')';

    return os;
}

/**
 * Check if all values of u is lower or equal respective values of v
 *
 */

template <typename T>
bool weak_dominates_vec(std::vector<T> &u, std::vector<T> &v)
{
    for (int i = 0; i < u.size(); i++)
    {
        if (u[i] > v[i])
        {
            return false;
        }
    }

    return true;
}

template <typename T>
bool weak_is_dominated_vec(std::vector<T> &u, std::vector<T> &v)
{
    for (int i = 0; i < u.size(); i++)
    {
        if (u[i] > v[i])
        {
            return false;
        }
    }

    return true;
}