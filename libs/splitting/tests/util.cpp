#include "util.h"

template <typename T>
bool compara_vetor(std::vector<T> &a, std::vector<T> &b)
{
    if (a.size() != b.size())
    {
        return false;
    }

    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }

    // todos os campos de a e b são iguais
    return true;
}

bool lambdas_are_equal(std::vector<std::vector<MyLabel>> base, Lambda lambda)
{
    bool not_found = true;
    for (int i = 0; i < base.size(); ++i)
    {
        if (lambda[i].size() != base[i].size())
        {
            return false;
        }

        // itera por cada elemento do front
        for (auto &label_base : base[i])
        {
            not_found = true;
            // verifica se tem alguem que está sendo comparado que seja igual
            for (auto &label : lambda[i])
            {
                bool comp = (label_base.distDem[0] == label.distance() &&
                             label_base.distDem[1] == label.demand() &&
                             compara_vetor(label_base.depots, label.used_trucks()));

                if (comp)
                {
                    not_found = false;
                    break;
                }
            }

            if (not_found)
            {
                return false;
            }
        }
    }

    return true;
}