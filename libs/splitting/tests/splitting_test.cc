#include <gtest/gtest.h>
#include "../mommurp.h"
#include "util.h"

TEST(SplittingTest, Case1_True)
{
    std::vector<std::vector<float>> dist_nodes_demand = {
        {0, 5, 7, 8, 20, 15, 17},
        {5, 0, 8, 9, 16, 14, 13},
        {7, 8, 0, 10, 14, 15, 12},
        {8, 9, 10, 0, 8, 8, 9},
        {20, 16, 14, 8, 0, 5, 7},
        {15, 14, 15, 8, 5, 0, 6},
        {17, 13, 12, 9, 7, 6, 0}};

    std::vector<std::vector<float>> dist_nodes_depot = {
        {3, 4, 5, 8, 15, 18, 19},
        {20, 15, 13, 9, 5, 4, 3}};

    std::vector<int> big_tour = {0, 1, 2, 3, 4, 5, 6};
    std::vector<float> demanda = {10, 20, 30, 15, 10, 20, 25};
    std::vector<int> max_number_vehicles = {2, 2};
    float max_load = 35;
    float max_dist_between_nodes = 25;

    std::vector<std::vector<MyLabel>> esperado(
        {{
             {{0, 0}, {0, 0}},

         },
         {
             {{3, 10}, {1, 0}},
             {{20, 10}, {0, 1}},
         },
         {
             {{8, 30}, {1, 0}},
             {{25, 30}, {0, 1}},
             {{7, 30}, {2, 0}},
         },
         {
             {{13, 60}, {2, 0}},
             {{21, 60}, {1, 1}},
             {{38, 60}, {0, 2}},
         },
         {
             {{22, 75}, {2, 1}},
             {{30, 75}, {1, 2}},
         },
         {
             {{30, 85}, {2, 1}},
             {{38, 85}, {1, 2}},
             {{27, 85}, {2, 2}},
         },
         {
             {{32, 105}, {2, 2}},
         },
         {}});

    auto [result, map] = splitting(dist_nodes_demand, dist_nodes_depot, big_tour,
                                   demanda, max_number_vehicles, max_load, max_dist_between_nodes);
    
    EXPECT_TRUE(lambdas_are_equal(esperado, result));
}

TEST(SplittingTest, Case1_False_front_4)
{
    std::vector<std::vector<float>> dist_nodes_demand = {
        {0, 5, 7, 8, 20, 15, 17},
        {5, 0, 8, 9, 16, 14, 13},
        {7, 8, 0, 10, 14, 15, 12},
        {8, 9, 10, 0, 8, 8, 9},
        {20, 16, 14, 8, 0, 5, 7},
        {15, 14, 15, 8, 5, 0, 6},
        {17, 13, 12, 9, 7, 6, 0}};

    std::vector<std::vector<float>> dist_nodes_depot = {
        {3, 4, 5, 8, 15, 18, 19},
        {20, 15, 13, 9, 5, 4, 3}};

    std::vector<int> big_tour = {0, 1, 2, 3, 4, 5, 6};
    std::vector<float> demanda = {10, 20, 30, 15, 10, 20, 25};
    std::vector<int> max_number_vehicles = {2, 2};
    float max_load = 35;
    float max_dist_between_nodes = 25;

    std::vector<std::vector<MyLabel>> esperado(
        {{
             {{0, 0}, {0, 0}},

         },
         {
             {{3, 10}, {1, 0}},
             {{20, 10}, {0, 1}},
         },
         {
             {{8, 30}, {1, 0}},
             {{25, 30}, {0, 1}},
             {{7, 30}, {2, 0}},
         },
         {
             {{13, 60}, {2, 0}},
             {{21, 60}, {1, 1}},
             {{38, 60}, {0, 2}},
         },
         {
             {{22, 75}, {2, 0}}, // {{22, 75}, {2, 1}}, // o correto
             {{30, 75}, {1, 2}},
         },
         {
             {{30, 85}, {2, 1}},
             {{38, 85}, {1, 2}},
             {{27, 85}, {2, 2}},
         },
         {
             {{32, 105}, {2, 2}},
         },
         {}});

    auto [result, map] = splitting(dist_nodes_demand, dist_nodes_depot, big_tour,
                                   demanda, max_number_vehicles, max_load, max_dist_between_nodes);
    
    EXPECT_FALSE(lambdas_are_equal(esperado, result));
}