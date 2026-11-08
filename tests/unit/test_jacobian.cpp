#include <gtest/gtest.h>
#include "brusselator.hpp"

TEST(JacobianTest, MatrixDimensions)
{
    SimulationConfig config;

    config.nx=2;
    config.ny=2;
    config.N=4;

    config.dt=0.01;
    config.reactionEnabled=true;

    config.A_tilde.resize(2*config.N, 2*config.N);
    config.A_tilde.setZero();

    Eigen::VectorXd Cguess = Eigen::VectorXd::Ones(2*config.N);

    auto J= brusselator::assembleJacobian(Cguess, config);

    EXPECT_EQ(J.rows(), 8);
    EXPECT_EQ(J.cols() ,8);

}

TEST(JacobianTest, ReactionJacobianEntries)
{
    SimulationConfig config;

    config.nx=2;
    config.ny=2;
    config.N= config.nx * config.ny;

    config.Ca=1.0;
    config.Cb=3.0;
    config.dt=0.01;

    config.reactionEnabled=true;

    config.A_tilde.resize(2*config.N, 2*config.N);
    config.A_tilde.setZero();

    Eigen::VectorXd Cguess = Eigen::VectorXd::Ones(2*config.N);

    auto J= brusselator::assembleJacobian(Cguess, config);

    //testing the local jacobian at cell 0
    EXPECT_NEAR(J.coeff(0,0),1.02,1e-12);
    EXPECT_NEAR(J.coeff(0,4),-0.01,1e-12);
    EXPECT_NEAR(J.coeff(4,0),-0.01,1e-12);
    EXPECT_NEAR(J.coeff(4,4),1.01,1e-12);

}

// testing when the reaction are disabled


TEST(JacobianTest, IdentityWhenReactionDisabled)
{
    SimulationConfig config;

    config.nx=2;
    config.ny=2;
    config.N=4;

    config.dt=0.01;
    config.reactionEnabled = false;

    config.A_tilde.resize(2*config.N, 2*config.N);
    config.A_tilde.setZero();


    Eigen::VectorXd Cguess= Eigen::VectorXd::Ones(2*config.N);
    
    auto J= brusselator::assembleJacobian(Cguess, config);

    Eigen::MatrixXd Identity = Eigen::MatrixXd::Identity(8,8);

    Eigen::MatrixXd difference = Eigen::MatrixXd(J) - Identity;

    EXPECT_NEAR(difference.norm(), 0.0, 1e-12);
}
