#include "util.h"
#include <iostream>

// // template <typename T>
// void Log(std::string const &message, T val)
// {
//     std::cout << "DEBUG >> " << message << " : " << val << std::endl;
// }

// void Log(std::string const &message)
// {
//     Log(message, "");
// }

void print_linha()
{
    std::cout << "------------------------------------" << std::endl;
}
void print_linha(int n_linhas)
{
    for (int i = 0; i < n_linhas; i++)
    {
        print_linha();
    }
}

void print_lambda(const std::vector<std::list<Label>> &lambda)
{
    // print_linha(2);
    int i = 0;
    for (auto &labels : lambda)
    {
        std::cout << ">> node: " << i << std::endl;
        for (auto &label : labels)
        {
            std::cout << label << std::endl;
        }
        // print_linha();
        ++i;
    }
    // print_linha(2);
    std::cout << "------------------------------------" << std::endl;
    std::cout << "------------------------------------" << std::endl;
}