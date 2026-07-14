#pragma once
#include <Eigen/Dense>
#include "simconfig.hpp"

namespace integrate {
    /**
    * @brief Classical Runge-Kutta method.
    *
    * Computes the next state using the classical Runge-Kutta method 
    * for solving ordinary differential equations (ODEs). 
    * This method is explicit and fourth-order accurate.
    *
    * @param f Right hand side of the ODE system dC/dt = f(C)
    * @param Cn The current state vector.
    * @param dt The time step size.
    * @return The state vector at the next time step.
    */
    Eigen::VectorXd rk4(
        const std::function<Eigen::VectorXd(const Eigen::VectorXd& C)>& f,
        const Eigen::VectorXd& Cn, 
        const SimulationConfig& config
        );

    /**
    * @brief Explicit Euler method.
    *
    * Computes the next state using the explicit Euler method for solving
    * ordinary differential equations (ODEs). 
    * This method is first-order accurate.
    *
    * @param f Right hand side of the ODE system dC/dt = f(C)
    * @param Cn The current state vector.
    * @param dt The time step size.
    * @return The state vector at the next time step.
    */
    Eigen::VectorXd Euler(
        const std::function<Eigen::VectorXd(const Eigen::VectorXd& C)>& f,
        const Eigen::VectorXd& Cn,
        const SimulationConfig& config
    );




    Eigen::VectorXd implicitEuler(const Eigen::VectorXd& Cold,
            const SimulationConfig& config);

} // end namespace integrate
