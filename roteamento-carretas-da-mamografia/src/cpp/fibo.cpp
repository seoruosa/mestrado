#include "fibo.h"
#include <vector>
#include <iostream>

int fibo(int n)
{
    if (n == 0 || n == 1)
    {
        return n;
    }

    return fibo(n - 1) + fibo(n - 2);
}

template <typename T>
T fibo_pd(int n, std::vector<T>& A)
{
    if (n == 0 || n == 1)
    {
        return n;
    }
    
    A[0] = 0;
    A[1] = 1;
    for (size_t i = 2; i < n; i++)
    {
        A[i] = A[i - 1] + A[i - 2];
    }
    
    return A[n - 1];
}

unsigned long long int fibo_pd(int n)
{
    std::vector<long long int> A(n);
    return fibo_pd(n, A);
}

int main()
{
    for (int i = 0; i < 10; i++)
    {
        std::cout << i << " -> " << fibo(i) << std::endl;
    }
    
}

