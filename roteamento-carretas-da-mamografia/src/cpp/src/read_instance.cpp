#include <iostream>
#include <fstream>
#include <regex>
#include <cmath>
#include "read_instance.h"


enum DistType {EUC_2D = 0, DIST = 1, NOT_FOUND = -1};
DistType dist_type(std::string &edge_weight_type)
{
    DistType mode;

    auto exist_on_type = [edge_weight_type](const auto word){return (edge_weight_type.find(word) != std::string::npos);};

    if (exist_on_type("DIST"))
    {
        mode = DistType::DIST;
    }
    else if (exist_on_type("EUC_2D"))
    {
        mode = DistType::EUC_2D;
    }
    else
    {
        mode = DistType::NOT_FOUND;
    }
    
    return mode;    
}

void read_instance(std::vector<std::vector<float>> &dist_nodes_nodes, std::vector<std::vector<float>> &dist_depots_nodes,
                   std::vector<float> &demand, int &capacity, const std::string &name, const std::string &filepath)

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
                    else if (field == "EDGE_WEIGHT_TYPE")
                    {
                        EDGE_WEIGHT_TYPE = value;
                        // std::cout << "EDGE_WEIGHT_TYPE: " <<  EDGE_WEIGHT_TYPE << std::endl;
                        mode = dist_type(EDGE_WEIGHT_TYPE);
                    }
                }
            }
            else if ((std::regex_search(string_line, match, name_reg) & mode != DistType::NOT_FOUND))
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

std::vector<std::vector<int>> get_int_matrix(std::ifstream &file, int lines, int columns, int skip_columns)
{
    std::string string_line;
    std::regex number_reg("\\d+");
    auto node_coord = std::vector<std::vector<int>>(lines, std::vector<int>(columns, 0));

    for (int i = 0; i < lines; i++)
    {
        std::getline(file, string_line);

        auto words_begin = std::sregex_iterator(string_line.begin(), string_line.end(), number_reg);
        auto words_end = std::sregex_iterator();

        int j = 0;
        for (auto ind = words_begin; ind != words_end; ++ind)
            while ((j < (columns + skip_columns)) & (ind != words_end))
            {
                if (j >= skip_columns)
                {
                    auto number_match = *ind;
                    node_coord[i][j - skip_columns] = std::stoi(number_match.str());
                }
                j++;
                ++ind;
            }
    }

    return node_coord;
}

std::vector<std::vector<float>> get_float_matrix(std::ifstream &file, int lines, int columns, int skip_columns)
{
    std::string string_line;
    std::regex number_reg("\\d+\\.{0,1}\\d*");
    auto node_coord = std::vector<std::vector<float>>(lines, std::vector<float>(columns, 0));

    for (int i = 0; i < lines; i++)
    {
        std::getline(file, string_line);

        auto words_begin = std::sregex_iterator(string_line.begin(), string_line.end(), number_reg);
        auto words_end = std::sregex_iterator();

        int j = 0;
        for (auto ind = words_begin; ind != words_end; ++ind)
            while ((j < (columns + skip_columns)) & (ind != words_end))
            {
                if (j >= skip_columns)
                {
                    auto number_match = *ind;
                    node_coord[i][j - skip_columns] = std::stof(number_match.str());
                }
                j++;
                ++ind;
            }
    }

    return node_coord;
}

template <class T>
float dist_euc(T &a, T &b)
{
    float d = 0.0F;
    const int size = a.size();

    for (auto i = 0; i < size; i++)
    {
        d = d + (a[i] - b[i]) * (a[i] - b[i]);
    }

    return std::sqrt(d);
}

std::vector<std::vector<float>> dist_matrix(std::vector<std::vector<int>> &a)
{
    std::vector<std::vector<float>> dist(a.size(), std::vector<float>(a.size(), 0));

    int i = 0;
    for (auto vec1 = a.begin(); vec1 != a.end(); ++vec1)
    {
        int j = i + 1;
        for (auto vec2 = vec1 + 1; vec2 != a.end(); vec2++)
        {
            float dist_points = dist_euc(*vec1, *vec2);
            dist[i][j] = dist_points;
            dist[j][i] = dist_points;
            ++j;
        }
        ++i;
    }
    return dist;
}

std::vector<std::vector<float>> dist_matrix(std::vector<std::vector<int>> &nodes, std::vector<std::vector<int>> &depots)
{
    std::vector<std::vector<float>> dist(depots.size(), std::vector<float>(nodes.size(), 0));

    int i = 0;
    for (auto depot = depots.begin(); depot != depots.end(); ++depot)
    {
        int j = 0;
        for (auto node = nodes.begin(); node != nodes.end(); ++node)
        {
            float dist_points = dist_euc(*depot, *node);
            dist[i][j] = dist_points;
            ++j;
        }
        ++i;
    }
    return dist;
}
