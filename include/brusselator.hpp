#pragma once
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include "simconfig.hpp"
namespace brusselator {

    Eigen::VectorXd initializeState(
            const SimulationConfig& config);

    /**
    * @brief ODE System of the Brusselator.
    *
    * @param C_tilde Current state vector
    * @return The derivative vector dC_tilde / dt
    */
    Eigen::VectorXd odeSystem(
        const Eigen::VectorXd& C_tilde,
        const Eigen::SparseMatrix<double>& A_tilde,
        const SimulationConfig& config
        );

    /**
     * @buildA builds the laplacian of coefficients of
     * concentrations C. The size of the matrix is NxN.
     * @config is the object of type class SimulationConfig
     * @returns the sparse matrix containing the coefficients of the central differencing scheme. Each row in this matrix
     * corresponds to a single cell in the grid
     */

    Eigen::SparseMatrix<double> buildA(
            const SimulationConfig& config
            );


    /**
     * @buildAtilde builds the A_tilde matrix from A 
     * which helps to write the governing equations in a more
     * compact notation A_tilde= [D1A 0
     *                            0  D2A]
     *The size of A_tilde will be 2Nx2N.
     */

    Eigen::SparseMatrix<double> buildAtilde(const Eigen::SparseMatrix<double>& A, const SimulationConfig& config);



    /**
     * the computeReaction function calculates the reaction 
     * part of the brusselator non linear model
     */


    Eigen::VectorXd computeReaction(const Eigen::VectorXd& C,const SimulationConfig& config);


    Eigen::VectorXd computeResidual(const Eigen::VectorXd& Cguess, const Eigen::VectorXd& Cold, const SimulationConfig& config);

    Eigen::SparseMatrix<double> assembleJacobian(const Eigen::VectorXd& Cguess, const SimulationConfig& config);
}
