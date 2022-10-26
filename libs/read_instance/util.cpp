#include "util.h"

#include <regex>
#include <cmath>

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