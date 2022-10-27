#pragma once
#include <string>
#include <vector>
#include <list>
#include "label.h"

// template <typename T>
// void Log(std::string const &message, T val);
// void Log(std::string const &message);
void print_linha();
void print_linha(int n_linhas);
void print_lambda(const std::vector<std::list<Label>> &lambda);
