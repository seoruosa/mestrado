#include <iostream>
// #include <sstream>
// #include <filesystem>
// #include <chrono>
#include <string>
#include "performance.h"

namespace perf
{
    namespace time
    {
        std::chrono::high_resolution_clock::time_point start()
        {
            return std::chrono::high_resolution_clock::now();
        }

        // template <typename Ratio=std::micro>
        std::chrono::duration<double, std::micro> duration(std::chrono::high_resolution_clock::time_point start_)
        {
            auto end_gen = start();
            std::chrono::duration<double, std::micro> duration(end_gen - start_);

            return duration;
        }
    }

    void save_measurement(std::string const &name, std::chrono::duration<double, std::micro> duration, std::ostream &file)
    {
        file << name << " : " << (duration.count()/1e6) << std::endl;
    }

    void save_gen(int generation, std::ostream &file)
    {
        file << "********************************" << std::endl;
        file << "generation : " << generation << std::endl;
    }
}