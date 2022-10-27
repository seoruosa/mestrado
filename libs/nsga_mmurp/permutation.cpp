#include <algorithm>
#include <chrono>

#include "permutation.h"
// #include "random_generation.h"

#include "seeds.h"

/**
 * @brief create a function increment that could be initialized with any integer value
 * 
 */
class increment {
    private:
        int value;
    public:
        increment(): value(0) {}
        increment(int v): value(v) {}
        int operator() () { return value++; }
};

// TODO create a generic version
// template <class T>
std::vector<int> generate_int_values(const int first, const int n)
{
    std::vector<int> vec(n);
    std::generate_n(vec.begin(), n, increment(first));
    return vec;
}

std::vector<int> generate_int_values(const int n)
{
    return generate_int_values(0, n);
}

std::vector<std::vector<int>> random_pairs(int n)
{
    std::vector<int> a(generate_int_values(n));
    auto b(a);

    std::shuffle(a.begin(), a.end(), RND_ENGINE);
    std::shuffle(b.begin(), b.end(), RND_ENGINE);

    std::vector<std::vector<int>> out(n, std::vector<int>(2));

    for (int i = 0; i < n; i++)
    {
        out[i] = {a[i], b[i]};
    }
    
    return out;
}