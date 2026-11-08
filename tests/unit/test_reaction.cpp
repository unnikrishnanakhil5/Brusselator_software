#include <gtest/gtest.h>
#include "brusselator.hpp"

// the function below tests computeReaction() with the initial 
// conditions C1, C2 equal to 1 with reactions enabled

TEST(ReactionTest, ComputesCorrectReactionTerms)
{
    // Creating the simulation configuration
    SimulationConfig config;

    config.nx = 2;
    config.ny = 2;
    config.N = config.nx * config.ny; // total number of grid cells
    config.Ca = 1.0;
    config.Cb = 3.0;

    config.reactionEnabled = true;

    //Create an input concentration field
    Eigen::VectorXd C= Eigen::VectorXd::Ones(2*config.N);

    //Calculate the reaction terms
    Eigen::VectorXd reaction = brusselator::computeReaction(C, config);

    // Checking vector dimensions

    EXPECT_EQ(reaction.size(), 2 * config.N);

    // checking expected reaction values
    for (int i =0; i < config.N; i++)
    {
        EXPECT_NEAR(reaction(i), -2.0, 1e-12);
        EXPECT_NEAR(reaction(i+config.N), 2, 1e-12);
    }
}

// below is the test function to test with reactions disabled, the expected result must be a zero vector as I have initialized it so inside the function definition

TEST(ReactionTest, ComputeCorrectReactionTerms)
{
    SimulationConfig config;

    config.nx = 2;
    config.ny = 2;
    config.N = config.nx * config.ny;
    config.Ca = 1;
    config.Cb = 3;

    config.reactionEnabled= false;

    Eigen::VectorXd C= Eigen::VectorXd::Ones(2 * config.N);

    Eigen::VectorXd reaction = brusselator::computeReaction(C, config);

    EXPECT_EQ(reaction.size(), 2*config.N);

    EXPECT_TRUE(reaction.isZero(1e-12));
}
