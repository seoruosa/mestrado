#pragma once
// #include <iostream>
// #include <sstream>
// #include <filesystem>
#include <chrono>
// #include <string>

namespace perf
{
    namespace time
    {
        std::chrono::high_resolution_clock::time_point start();

        // template <typename Ratio=std::micro>
        std::chrono::duration<double, std::micro> duration(std::chrono::high_resolution_clock::time_point start_);
        
    }

    void save_measurement(std::string const &name, std::chrono::duration<double, std::micro> duration, std::ostream &file);
    void save_gen(int generation, std::ostream &file);
}