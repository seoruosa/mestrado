#pragma once

#include <vector>
#include "../mommurp.h"

struct MyLabel
{
    std::vector<float> distDem;
    std::vector<int> depots;
};

template <typename T>
bool compara_vetor(std::vector<T> &a, std::vector<T> &b);

bool lambdas_are_equal(std::vector<std::vector<MyLabel>> base, Lambda lambda);
bool lambdas_are_equal(Lambda base, Lambda lambda);