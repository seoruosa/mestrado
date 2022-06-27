#include <vector>
#include "nsgaii.cpp"

int main()
{
    // NSGAII(int size_pop, int number_generations, auto f, int number_obj, float mutation_rate)
    auto SCH = [](auto a)
    {
        auto x = a[0];
        return std::vector<float>({x * x, (float)((x - 2.0) * (x - 2.0))});
    };

    auto kur = [](std::vector<float> x){

        float f1 = 0.0;
        float f2 = 0.0;
        auto m = x.size();

        for (size_t i = 0; i < m - 1; i++)
            f1 = f1 - 10* std::exp(-0.2 * std::sqrt(x[i]*x[i] + x[i + 1]*x[i + 1]));
        
        for (size_t i = 0; i < m; i++)
            f2 = f2 + std::pow(std::abs(x[i]), 0.8) + 5*std::sin(x[i]*x[i]*x[i]);
        
        return std::vector<float>({f1, f2});
    };

    // NSGAII(50, 1000, sch, 1, 0.1);
    
    NSGAII(500, 500, kur, 3, 0.1);
}