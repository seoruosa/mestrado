#include "mmurp_data.h"
#include "mmurp.h"

MMURP_data::MMURP_data(std::string instance_path) : instance_path(instance_path)
{
    // TODO need to modify the read_instance to this shouldnt be necessary
    int int_vehicle_capacity;

    read_instance(MMURP_data::dist_nodes_nodes, MMURP_data::dist_depots_nodes,
                    MMURP_data::demand, int_vehicle_capacity, MMURP_data::number_vehicles, 
                    MMURP_data::max_travel_dist, "", instance_path);
    
    MMURP_data::vehicle_capacity = float(int_vehicle_capacity);
    MMURP_data::n_clients = MMURP_data::demand.size();
    MMURP_data::n_depots = MMURP_data::dist_depots_nodes.size();
}

MMURP_data::~MMURP_data()
{
}