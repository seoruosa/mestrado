#include <iostream>
#include <fstream>
#include <regex>
// #include <cmath>
#include "mmurp.h"
#include "util.h"

enum DistType
{
    EUC_2D = 0,
    DIST = 1,
    NOT_FOUND = -1
};
DistType dist_type(std::string &edge_weight_type)
{
    DistType mode = DistType::NOT_FOUND;

    auto exist_on_type = [edge_weight_type](const auto word)
    { return (edge_weight_type.find(word) != std::string::npos); };

    if (exist_on_type("DIST"))
    {
        mode = DistType::DIST;
    }
    else if (exist_on_type("EUC_2D"))
    {
        mode = DistType::EUC_2D;
    }

    return mode;
}

void read_instance(std::vector<std::vector<float>> &dist_nodes_nodes, std::vector<std::vector<float>> &dist_depots_nodes,
                   std::vector<float> &demand, int &capacity, std::vector<int> &number_vehicles, float &max_travel_dist,
                   const std::string &name, const std::string &filepath)

{
    std::ifstream file(filepath);
    std::string string_line;

    std::smatch match;
    std::regex two_points_reg("[\\s\\t]*:[\\s\\t]*");
    std::regex name_reg("\\w+");
    std::regex number_reg("\\d+");

    int NODES;
    int DEPOTS;

    std::vector<std::vector<int>> depots_coord;
    std::vector<std::vector<int>> nodes_coord;

    std::string EDGE_WEIGHT_TYPE;

    DistType mode;

    auto get_number = [&](std::string a)
    {
        std::smatch n_match;
        float output(INT16_MAX);

        if (std::regex_search(a, n_match, number_reg))
        {
            output = std::stoi(n_match[0].str());
        }

        return output;
    };

    if (file.is_open())
    {
        while (file.good())
        {
            std::getline(file, string_line);

            if (std::regex_search(string_line, match, two_points_reg))
            {
                auto field = match.prefix().str();
                auto value = match.suffix().str();

                if (std::regex_search(field, match, name_reg))
                {
                    field = match[0].str();

                    if (field == "CAPACITY")
                    {
                        capacity = get_number(value);
                        // std::cout << "CAPACITY: " <<  capacity << std::endl;
                    }
                    else if (field == "NODES")
                    {
                        NODES = get_number(value);
                        // std::cout << "NODES: " <<  NODES << std::endl;
                    }
                    else if (field == "DEPOTS")
                    {
                        DEPOTS = get_number(value);
                        // std::cout << "DEPOTS: " <<  DEPOTS << std::endl;
                    }
                    else if (field == "MAX_TRAVEL_DIST")
                    {
                        max_travel_dist = get_number(value);
                    }
                    else if (field == "EDGE_WEIGHT_TYPE")
                    {
                        EDGE_WEIGHT_TYPE = value;
                        // std::cout << "EDGE_WEIGHT_TYPE: " <<  EDGE_WEIGHT_TYPE << std::endl;
                        mode = dist_type(EDGE_WEIGHT_TYPE);
                    }
                }
            }
            else if ((std::regex_search(string_line, match, name_reg) && mode != DistType::NOT_FOUND))
            {
                // std::cout << "ELSE IF: " <<  string_line << std::endl;
                auto section_name = match[0].str();

                if (section_name == "NODE_COORD_SECTION")
                {
                    if (mode == DistType::DIST)
                    {
                        dist_nodes_nodes = get_float_matrix(file, NODES, NODES, 0);
                    }
                    else if (mode == DistType::EUC_2D)
                    {
                        // TODO fetch a float matrix
                        nodes_coord = get_int_matrix(file, NODES, 2, 1);
                    }
                }
                else if (section_name == "DEMAND_SECTION")
                {
                    auto a = get_float_matrix(file, NODES, 1, 1);

                    for (auto &el : a)
                    {
                        demand.push_back(el[0]);
                    }

                    // std::cout << "DEMAND_SECTION: " << a.size() << " - " << a[0].size() << std::endl;
                }
                else if (section_name == "DEPOT_SECTION")
                {
                    if (mode == DistType::DIST)
                    {
                        dist_depots_nodes = get_float_matrix(file, DEPOTS, NODES + DEPOTS, 0);
                    }
                    else if (mode == DistType::EUC_2D)
                    {
                        depots_coord = get_int_matrix(file, DEPOTS, 2, 1);
                    }
                }
                else if (section_name == "DEPOT_VEHICLES_SECTION")
                {
                    auto a = get_int_matrix(file, DEPOTS, 1, 0);

                    for (auto &el : a)
                    {
                        number_vehicles.push_back(el[0]);
                    }
                }
            }
        }

        if (mode == DistType::EUC_2D)
        {
            dist_nodes_nodes = dist_matrix(nodes_coord);
            dist_depots_nodes = dist_matrix(nodes_coord, depots_coord);
        }
    }
    else
    {
        std::cout << "FILE IS NOT OPEN" << std::endl;
    }
}
