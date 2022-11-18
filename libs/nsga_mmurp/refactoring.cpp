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
protected:
    unsigned seed; // = std::chrono::system_clock::now().time_since_epoch().count();
    // std::default_random_engine RND_ENGINE = std::default_random_engine(seed);
    std::vector<std::vector<float>> pop_obj_val;
    int population_size;
    int number_generations;
    float mutation_rate;
    int number_objectives;
    unsigned seed;
public:
    NSGAII(int population_size, int number_generations, float mutation_rate, int number_objectives, unsigned seed);
    NSGAII(int population_size, int number_generations, float mutation_rate, int number_objectives);
    ~NSGAII();
    virtual void initialize() const = 0;
    virtual std::vector<Individual> & parent_selection() const = 0;
    virtual Individual & crossover(Individual & p1, Individual & p2) const = 0;
    virtual Individual & mutate(Individual & ind) const = 0;
    std::tuple<std::vector<std::vector<int>>, std::vector<int>> fast_non_dominated_sort(std::vector<std::vector<float>> &pop_obj_val);
    std::vector<int> crowding_distance_assignment(std::vector<std::vector<float>> &pop_obj_value, std::vector<int> &rank, 
        std::vector<int> &idx_vec);

    void run_default();
};

class Mutation
{
private:
    double mutation_rate_;
    unsigned seed_;
public:
    Mutation(unsigned seed);
    Mutation();
    virtual Individual & mutate(Individual & indiv) const = 0;
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
    virtual std::vector<float> & evaluate() const = 0;
};

class Problem
{
private:
    int m; // number of objectives
    
public:
    Problem(const std::string instance_path);
    ~Problem();
    int number_objectives();
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
*   * have an unique SEED for random generators
*/

// TODO logging stuff on algorithms

// https://refactoring.guru/design-patterns/builder -> builder
// https://refactoring.guru/design-patterns/template-method -> template

