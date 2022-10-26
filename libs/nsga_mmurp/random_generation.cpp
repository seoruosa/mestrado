// #include <chrono>
#include <algorithm>

#include <random>
#include <vector>
#include <chrono>

#include "random_generation.h"

std::vector<float> generate_random_vec(const int &size, std::uniform_real_distribution<> &unif)
{
    unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
    auto RND_ENGINE = std::default_random_engine(SEED);

    std::vector<float> vec(size);

    std::generate(vec.begin(), vec.end(), ([&]()
                                           { return unif(RND_ENGINE); }));

    return vec;
}

std::vector<float> generate_random_vec(const int &size)
{
    std::uniform_real_distribution<> unif(0.0, 1.0);

    return generate_random_vec(size, unif);
}

std::vector<float> generate_random_vec(const int &size, const float &min, const float &max)
{
    std::uniform_real_distribution<> unif(min, max);

    return generate_random_vec(size, unif);
}

void generate_random_mat(std::vector<std::vector<float>> &mat, std::uniform_real_distribution<> &unif)
{
    unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
    auto RND_ENGINE = std::default_random_engine(SEED);

    for (auto &line : mat)
    {
        std::generate(line.begin(), line.end(), ([&]()
                                                 { return unif(RND_ENGINE); }));
    }
}

std::vector<std::vector<float>> generate_random_mat(const int &rows, const int &columns, std::uniform_real_distribution<> &unif)
{
    std::vector<std::vector<float>> mat(rows, std::vector<float>(columns));

    generate_random_mat(mat, unif);

    return mat;
}
