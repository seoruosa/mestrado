#include <vector>
#include <iostream>
#include <cmath>

template <class T>
void print_vec(T &vec)
{
    for (auto el : vec)
    {
        std::cout << el << " - ";
    }
    std::cout << std::endl;
}

template <class T>
void print_mat(T &mat)
{
    for (auto &el : mat)
    {
        print_vec(el);
    }
}

template <class T>
float dist_euc(T &a, T &b)
{
    float d = 0.0F;
    const int size = a.size();

    for (auto i = 0; i < size; i++)
    {
        d = d + (a[i] - b[i]) * (a[i] - b[i]);
    }

    return std::sqrt(d);
}

std::vector<std::vector<float>> dist_matrix(std::vector<std::vector<int>> &a)
{
    std::vector<std::vector<float>> dist(a.size(), std::vector<float>(a.size(), 0));
    
    int i = 0;
    for (auto vec1 = a.begin(); vec1 != a.end(); ++vec1)
    {
        int j = i + 1;
        for (auto vec2 = vec1 + 1; vec2 != a.end(); vec2++)
        {
            float dist_points = dist_euc(*vec1, *vec2);
            dist[i][j] = dist_points;
            dist[j][i] = dist_points;
            ++j;
        }
        ++i;
    }
    return dist;
}

void starline(int num = 45)
{
    for (int i = 0; i < num; i++)
        std::cout << '*';
    std::cout << std::endl;
}

void muda_valores(int &a, std::vector<int> &b)
{
    a = 10;
    b = {1,2,3,4};
}

int main()
{
    // std::vector<std::vector<int>> a({{1, 2}, {3, 3}, {4, 5}});
    
    // std::vector<std::vector<float>> dist(dist_matrix(a));

    // starline();
    // print_mat(dist);
    std::vector<int> vec(10, 1);
    int abc = 111;

    print_vec(vec);
    std::cout << abc << std::endl;
    starline();

    muda_valores(abc, vec);
    print_vec(vec);
    std::cout << abc << std::endl;

}