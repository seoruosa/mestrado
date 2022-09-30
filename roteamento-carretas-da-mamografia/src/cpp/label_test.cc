#include <gtest/gtest.h>
#include "label.h"

TEST(LabelTest, InitializeWithZeroes){
    int number_depots = 4;

    Label label(4);

    auto [distance, demand] = label.distDemand();
    
    EXPECT_EQ(distance, 0);
    EXPECT_EQ(demand, 0);

    for (auto &depot : label.used_trucks())
    {
        EXPECT_EQ(depot, 0) << "Expect the number of trucks on depot are zero";
    }
    
}

TEST(LabelTest, InitializeCorrectNumberOfDepots){
    int number_depots = 4;
    Label label(4);

    EXPECT_EQ(label.used_trucks().size(), number_depots);
}

TEST(LabelTest, IncrementDemand){
    int number_depots = 4;
    Label label(4);
    float inc = 10;

    label.inc_demand(inc);

    auto [distance, demand] = label.distDemand();

    EXPECT_EQ(demand, inc);
}

TEST(LabelTest, IncrementDistance){
    int number_depots = 4;
    Label label(4);
    float inc = 10;

    label.inc_distance(inc);

    auto [distance, demand] = label.distDemand();

    EXPECT_EQ(distance, inc);
}

TEST(LabelTest, NumberOfUsedTrucks){
    int number_depots = 4;
    Label label(4);


    for (int i = 0; i < number_depots; i++)
    {
        label.inc_num_vehicles_depot(i);
        EXPECT_EQ(label.num_vehicles_depot(i), 1) << "Should be 1";
    }
}

TEST(LabelTest, IncrementUsedTrucks){
    int number_depots = 4;
    Label label(4);
    int depot = 2;

    assert (depot < number_depots);

    label.inc_num_vehicles_depot(depot);


    for (int i = 0; i < number_depots; i++)
    {
        if (i == depot)
        {
            EXPECT_EQ(label.num_vehicles_depot(i), 1) << "Should be 1";
        }
        else
        {
            EXPECT_EQ(label.num_vehicles_depot(i), 0) << "Should be 0";
        }
    }
}

TEST(WeakDominationVecTest, DominatesEqual){
    std::vector<int> a = {1,2,3};

    EXPECT_TRUE(weak_dominates_vec(a, a));
}

TEST(WeakDominationVecTest, DominatesDifferent){
    std::vector<int> a = {1,2,3};
    std::vector<int> b = {1,2,4};

    EXPECT_TRUE(weak_dominates_vec(a, b));
}

TEST(WeakDominationVecTest, DoesntDominate){
    std::vector<int> a = {1,2,3};
    std::vector<int> b = {1,1,4};

    EXPECT_FALSE(weak_dominates_vec(a, b));
}

TEST(WeakDominationVecTest, Incomparable){
    std::vector<int> a = {1,3,4};
    std::vector<int> b = {1,4,3};

    EXPECT_FALSE(weak_dominates_vec(a, b));
}

TEST(WeakDominationVecTest, IncomparableReversed){
    std::vector<int> a = {1,3,4};
    std::vector<int> b = {1,4,3};

    EXPECT_FALSE(weak_dominates_vec(b, a));
}

TEST(WeakDominationVecTest, LabelAIsDominatedByLabelBCase2){
    std::vector<int> a = {1,1,0};
    std::vector<int> b = {0,1,0};

    EXPECT_FALSE(weak_dominates_vec(a, b));
}

TEST(WeakDominationVecTest, LabelADominatesLabelBCase2){
    std::vector<int> a = {0,1,0};
    std::vector<int> b = {1,1,0};

    EXPECT_TRUE(weak_dominates_vec(a, b));
}

TEST(LabelWeakDominationTest, DominatesEqual){
    Label a(4);
    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(2);

    EXPECT_TRUE(a.weak_dominates(a));
}

TEST(LabelWeakDominationTest, DominatesDifferentDistance){
    Label a(4);
    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(2);

    Label b(a);
    b.inc_distance(10);

    EXPECT_TRUE(a.weak_dominates(b));
}

TEST(LabelWeakDominationTest, DoesntDominateDifferentDistance){
    Label a(4);
    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(2);

    Label b(a);
    
    a.inc_distance(10);

    EXPECT_FALSE(a.weak_dominates(b));
}

TEST(LabelWeakDominationTest, DominatesDifferentDemand){
    Label a(4);
    int dk = 2;

    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(dk);

    Label b(a);
    b.inc_demand(-1);

    EXPECT_TRUE(a.weak_dominates(b));
}

TEST(LabelWeakDominationTest, DoesntDominateDifferentDemand){
    Label a(4);
    int dk = 2;

    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(dk);

    Label b(a);
    b.inc_demand(1);

    EXPECT_FALSE(a.weak_dominates(b));
}

TEST(LabelWeakDominationTest, DominatesDifferentUsedVehicles){
    Label a(4);
    int dk = 2;

    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(dk);

    Label b(a);
    b.inc_num_vehicles_depot(dk);

    EXPECT_TRUE(a.weak_dominates(b));
}

TEST(LabelWeakDominationTest, DoesntDominateDifferentUsedVehicles){
    Label a(4);
    int dk = 2;

    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(dk);

    Label b(a);
    a.inc_num_vehicles_depot(dk);

    EXPECT_FALSE(a.weak_dominates(b));
}

TEST(LabelWeakIsDominatedTest, Equal){
    Label a(4);
    int dk = 2;

    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(dk);

    EXPECT_TRUE(a.weak_is_dominated(a));
}

TEST(LabelWeakIsDominatedTest, DifferentNumVehicles){
    Label a(4);
    int dk = 2;

    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(dk);

    Label b(a);
    b.inc_num_vehicles_depot(dk);

    EXPECT_TRUE(b.weak_is_dominated(a));
}

TEST(LabelWeakIsDominatedTest, DifferentDistance){
    Label a(4);
    int dk = 2;

    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(dk);

    Label b(a);
    b.inc_distance(-1);

    EXPECT_TRUE(a.weak_is_dominated(b));
}

TEST(LabelWeakIsDominatedTest, DifferentDemand){
    Label a(4);
    int dk = 2;

    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(dk);

    Label b(a);
    b.inc_demand(1);

    EXPECT_TRUE(a.weak_is_dominated(b));
}

TEST(LabelWeakIsDominatedTest, SameDemandDifferentDistanceAndNumberOfVehicles){
    int n_depot = 3;
    Label a(n_depot);

    a.inc_distance(8);
    a.inc_demand(30);
    a.inc_num_vehicles_depot(0);

    Label testing(n_depot);

    testing.inc_distance(7);
    testing.inc_demand(30);
    testing.inc_num_vehicles_depot(0);
    testing.inc_num_vehicles_depot(1);

    EXPECT_FALSE(testing.weak_is_dominated(a));
}

TEST(LabelWeakIsDominatedTest, SameDemandDifferentDistanceAndNumberOfVehicles2){
    int n_depot = 3;
    Label a(n_depot);

    a.inc_distance(8);
    a.inc_demand(30);
    a.inc_num_vehicles_depot(1);

    Label testing(n_depot);

    testing.inc_distance(7);
    testing.inc_demand(30);
    testing.inc_num_vehicles_depot(0);
    testing.inc_num_vehicles_depot(1);

    EXPECT_FALSE(testing.weak_is_dominated(a));
}

TEST(LabelIsNotDominatedByTest, ListEquals){
    Label a(4);
    int dk = 2;

    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(dk);

    std::list<Label> labelsVec(3, a);

    EXPECT_FALSE(a.is_not_dominated_by(labelsVec));
}

TEST(LabelIsNotDominatedByTest, ListOfDominatedUsedVehicles){
    Label a(4);
    int dk = 2;

    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(dk);

    Label b(a);
    b.inc_num_vehicles_depot(dk);

    std::list<Label> labelsVec(3, b);

    EXPECT_TRUE(a.is_not_dominated_by(labelsVec));
}

TEST(LabelIsNotDominatedByTest, ListOfLabelsThatDominatesTestedLabel){
    Label a(4);
    int dk = 2;

    a.inc_demand(10);
    a.inc_distance(20);
    a.inc_num_vehicles_depot(dk);

    Label b(a);
    a.inc_num_vehicles_depot(dk);

    std::list<Label> labelsVec(3, b);

    EXPECT_FALSE(a.is_not_dominated_by(labelsVec));
}

TEST(LabelIsNotDominatedByTest, ListOfLabelsThatDoesntDominatesLabel){
    int n_depot = 3;
    Label a(n_depot);

    a.inc_distance(8);
    a.inc_demand(30);

    Label b(a);
    b.inc_num_vehicles_depot(1);
    // b: (8,30 | 0, 1, 0)

    a.inc_num_vehicles_depot(0);
    // a: (8,30 | 1, 0, 0)

    std::list<Label> labelsVec({a, b});

    Label testing(n_depot);

    testing.inc_distance(7);
    testing.inc_demand(30);
    testing.inc_num_vehicles_depot(0);
    testing.inc_num_vehicles_depot(1);
    // testing: (7,30 | 1, 1, 0)

    EXPECT_TRUE(testing.is_not_dominated_by(labelsVec));
}