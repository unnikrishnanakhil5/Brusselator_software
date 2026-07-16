#include "integrator.hpp"
#include "brusselator.hpp"
#include <iostream>

Eigen::VectorXd integrate::rk4(
    const std::function<Eigen::VectorXd(const Eigen::VectorXd& C)>& f,
    const Eigen::VectorXd& C,
    const SimulationConfig& config
) {
    // ToDo
    Eigen::VectorXd k1=f(C);
    Eigen::VectorXd k2=f(C+config.dt*k1/2);
    Eigen::VectorXd k3=f(C+config.dt*k2/2);
    Eigen::VectorXd k4=f(C+config.dt*k3);

    return C+(config.dt/6)*(k1+2*k2+2*k3+k4);

  }

Eigen::VectorXd integrate::Euler(
    const std::function<Eigen::VectorXd(const Eigen::VectorXd& C)>& f,
    const Eigen::VectorXd& C,
    const SimulationConfig& config
)
{
    return C+config.dt*f(C);

};

Eigen::VectorXd integrate::implicitEuler(const Eigen::VectorXd& Cold, const SimulationConfig& config, int timestep,
        std::ofstream& residualFile)
{
    // initial guess
    Eigen::VectorXd Cguess = Cold;


    //assemble jacobian
    Eigen::SparseMatrix<double> J= brusselator::assembleJacobian(Cguess, config);

    //factorize once
    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;

    solver.compute(J);

    const int maxNewtonIterations=20;
    const double tolerance=1e-8;

    for (int i=0; i<maxNewtonIterations;i++)
    {
        Eigen::VectorXd residual=brusselator::computeResidual(
                Cguess, Cold,config);

        residualFile << timestep << "\t"
            << i << "\t" << residual.norm() << "\n";
        if(residual.norm() < tolerance)
        {
            break;
        }

        Eigen::VectorXd delta = solver.solve(-residual);

        if (solver.info() != Eigen::Success)
        {
            std::cerr << "Newton linear solve failed!" << std::endl;
            break;
        }

        Cguess+=delta;
    }

    return Cguess;
}
