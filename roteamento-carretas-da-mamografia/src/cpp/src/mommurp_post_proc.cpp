#include <vector>
#include <string>
#include "read_instance.h"

int main(const int argc, const char *argv[])
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
    std::string solution_path = arguments[0];
    std::string instance_path = arguments[1];

    std::vector<std::vector<float>> all_solutions = all_solutions_pos_proc(solution_path, instance_path);

    std::cout << "dist" << ", " << "demand" << std::endl;
    for (auto &el : all_solutions)
    {
        std::cout << el[0] << ", " << el[1] << std::endl;
    }
}