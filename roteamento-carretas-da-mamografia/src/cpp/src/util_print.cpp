#include "util_print.h"
#include <iostream>


template <class T>
void print_vec(T &vec, const std::string sep)
{
    for (auto el = vec.begin(); el != vec.end(); ++el)
    {
        std::cout << *el;
        
        if ( el+1 != vec.end())
        {
            std::cout << sep;
        }
        
    }
    std::cout << std::endl;
}

template <class T>
void print_vec(T &vec)
{
    print_vec(vec, ", ");
}

template <class T>
void print_mat(T &mat)
{
    for (auto &el : mat)
    {
        print_vec(el);
    }
}

void starline() { starline(45); }

void starline(int num)
{
    for (int i = 0; i < num; i++)
        std::cout << '*';
    std::cout << std::endl;
}

void print_solution_csv(std::vector<std::vector<int>> pop, std::vector<std::vector<float>> pop_obj_val)
{
    int number_of_obj = pop_obj_val[0].size();

    for (int i = 0; i < (int) pop[0].size(); i++)
    {
        std::cout << "x_" << i << "; ";
    }
    
    for (int i = 0; i < number_of_obj - 1; i++)
    {
        std::cout << "f_" << i << "; ";
    }
    std::cout << "f_" << (number_of_obj - 1) << std::endl;
    
    for (size_t i = 0; i < pop.size(); i++)
    {
        for (auto &x : pop[i])
        {
            std::cout << x << "; ";
        }

        int j = 0;
        for (auto &x : pop_obj_val[i])
        {
            if (j < (int) (pop_obj_val[i]).size() - 1)
            {
                std::cout << x << "; ";
            }
            else
            {
                std::cout << x;
            }
            ++j;
        }

        std::cout << std::endl;
    }
}