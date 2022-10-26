#include <chrono>
#include <random>
#include <vector>

class MOEA
{
private:
    /* data */
public:
    MOEA(/* args */);
    ~MOEA();
};

class NSGAII
{
private:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine RND_ENGINE = std::default_random_engine(seed);
public:
    NSGAII(/* args */);
    ~NSGAII();
    
};

class Mutation
{
private:
    double mutation_rate_;
    int seed_;
public:
    Mutation(/* args */);
    virtual void mutate(Individual & indiv) const = 0;
    ~Mutation();
};


class Population
{
private:
    std::vector<Individual> individuals; // the type (std::vector<float>) is problem dependent
    std::vector<std::vector<float>> obj_value;
    
    // the following is specific for nsga
    std::vector<std::vector<int>> fronts;
    // these fields should be a Individual field or related to population? I believe population, because 
    //      this metrics is related to the relationship of individuals 
    // if these fields was present on individual will be easier to use them...
    // fronts could be a vector of vector of pointers (std::vector<std::vector<Individual&>> fronts;)
    std::vector<float> rank; 
    std::vector<float> distance;

public:
    Population(/* args */);
    ~Population();
};

class Individual
{
private:
    /* data */
public:
    Individual(/* args */);
    ~Individual();
};


/*
* [PD] => Problem Dependent
*   ************************************************************************
*   * generate initial population [PD]
*   * Evaluate objective values
*       * the function that evaluates the objective value of individual [PD]
*       * evaluate obj val to all population
*   * generate children
*       * Selection (Binary tournament)
*       * recombination [PD]
*       * mutation [PD]
*   * Fast non dominated sort
*   * Crowding distance assignment
*   * Stopping criteria
*   
*   
*   ************************************************************************
*   * logging data
*/

// TODO logging stuff on algorithms

// https://refactoring.guru/design-patterns/builder -> builder
// https://refactoring.guru/design-patterns/template-method -> template