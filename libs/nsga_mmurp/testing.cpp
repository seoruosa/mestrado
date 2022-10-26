#include "read_instance.h"
#include "util_print.cpp"
#include <iostream>

int main()
{

    // std::string filepath = "/opt/project/Instancias/Vrp-Set-A/A-n50-m4-Q80.vrp";
    std::string filepath = "../../../Instancias/Vrp-Set-A/A-n50-m4-Q80.vrp";

    
    int CAPACITY;

    std::vector<std::vector<int>> nodes_coord;
    std::vector<std::vector<int>> demand_coord;
    std::vector<std::vector<int>> depots_coord;

    read_instance(nodes_coord, demand_coord, depots_coord, CAPACITY, filepath);
    
    std::vector<std::vector<float>> dist_nodes_mat;
    std::vector<std::vector<float>> dist_depots_nodes_mat;

    dist_nodes_mat = dist_matrix(nodes_coord);
    dist_depots_nodes_mat = dist_matrix(nodes_coord, depots_coord);
    
    // print_mat(dist_depots_nodes_mat);
    // std::cout << "***************************************" << std::endl;
    // print_mat(dist_nodes_mat);
    print_mat(demand_coord);
    
}