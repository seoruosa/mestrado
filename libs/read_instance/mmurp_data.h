#pragma once

#include <string>
#include <vector>

class MMURP_data
{
public:
    MMURP_data(std::string path);
    ~MMURP_data();

    // path of the instance   
    std::string instance_path;

    // distance between the clients
    std::vector<std::vector<float>> dist_nodes_nodes;

    // distance between the clients and depots
    std::vector<std::vector<float>> dist_depots_nodes;

    // demand of each client
    std::vector<float> demand;

    // number of vehicles of each depot
    std::vector<int> number_vehicles;

    // capacity of vehicles
    float vehicle_capacity;

    // max allowed travelling distance between 2 clients
    float max_travel_dist;

    // number of clients
    int n_clients;

    // number of depots
    int n_depots;
};