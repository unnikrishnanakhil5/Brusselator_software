#include <gtest/gtest.h>
#include <iostream>
#include "brusselator.hpp"

TEST(LaplacianTest, MatrixDimensions) 
// function to test the dimension of the matrix
{
    SimulationConfig config;

    config.nx=40;
    config.ny=40;

    auto A= brusselator::buildA(config);

    EXPECT_EQ(A.rows(),1600);
    EXPECT_EQ(A.cols(),1600);

}

TEST(LaplacianTest, InteriorCoefficients)
// function to test the interior coefficients for 
// cells other than the boundary cells
{
    SimulationConfig config;

    config.dx = 0.005;

    auto A = brusselator::buildA(config);

    int k  = 41; 

    EXPECT_NEAR(A.coeff(k,k),-16e+4,1e-12);

    EXPECT_NEAR(A.coeff(k,1),4e+4,1e-12);
    EXPECT_NEAR(A.coeff(k,40),4e+4,1e-12);
    EXPECT_NEAR(A.coeff(k,42),4e+4,1e-12);
    EXPECT_NEAR(A.coeff(k,81),4e+4,1e-12);

}

TEST(LaplacianTest, ConstantFieldProducesZero)
// function to test the neumann boundary condition, using the 
// mathematical property of vanishing gradients for constant
// concentrations  
{
    SimulationConfig config;

    auto A = brusselator::buildA(config);

    Eigen::VectorXd C = Eigen::VectorXd::Ones(1600);

    Eigen::VectorXd result = A * C;

    EXPECT_NEAR(result.norm(),0.0,1e-12);
}
