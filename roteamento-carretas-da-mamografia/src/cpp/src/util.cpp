#include "util.h"
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
