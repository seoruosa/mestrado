#include "util_print.h"
#include <iostream>

#include <chrono>
#include <ctime>

template <class T>
void print_vec(T &vec, const std::string sep)
{
    for (auto el = vec.begin(); el != vec.end(); ++el)
    {
        std::cout << *el;

        if (el + 1 != vec.end())
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

void print_solution_csv(std::vector<std::vector<int>> &pop, std::vector<std::vector<float>> &pop_obj_val, std::ostream &cout)
{
    int number_of_obj = pop_obj_val[0].size();

    for (int i = 0; i < (int)pop[0].size(); i++)
    {
        cout << "x_" << i << "; ";
    }

    for (int i = 0; i < number_of_obj - 1; i++)
    {
        cout << "f_" << i << "; ";
    }
    cout << "f_" << (number_of_obj - 1) << std::endl;

    for (size_t i = 0; i < pop.size(); i++)
    {
        for (auto &x : pop[i])
        {
            cout << x << "; ";
        }

        int j = 0;
        for (auto &x : pop_obj_val[i])
        {
            if (j < (int)(pop_obj_val[i]).size() - 1)
            {
                cout << x << "; ";
            }
            else
            {
                cout << x;
            }
            ++j;
        }

        cout << std::endl;
    }
}

void print_solution_csv(std::vector<std::vector<int>> &pop, std::vector<std::vector<float>> &pop_obj_val)
{
    print_solution_csv(pop, pop_obj_val, std::cout);
}

void print_obj_val(std::vector<std::vector<float>> &pop_obj_val)
{
    auto &cout = std::cout;

    int number_of_obj = pop_obj_val[0].size();

    for (int i = 0; i < number_of_obj - 1; i++)
    {
        cout << "f_" << i << "; ";
    }
    cout << "f_" << (number_of_obj - 1) << std::endl;

    for (size_t i = 0; i < pop_obj_val.size(); i++)
    {
        int j = 0;
        for (auto &x : pop_obj_val[i])
        {
            if (j < (int)(pop_obj_val[i]).size() - 1)
            {
                cout << x << "; ";
            }
            else
            {
                cout << x;
            }
            ++j;
        }

        cout << std::endl;
    }
}

std::string time_now(const char *fmt)
{
    // https://www.programiz.com/cpp-programming/library-function/ctime/strftime
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    char date_string[100];

    std::strftime(date_string, 100, fmt, std::localtime(&time));
    std::string out(date_string);

    return out;
};

std::string time_now()
{
    return time_now("%Y-%m-%d_%H:%M:%S");
}