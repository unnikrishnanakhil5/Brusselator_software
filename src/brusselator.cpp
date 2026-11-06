#include "brusselator.hpp"
#include <iostream>

//In order for the std::vector<Eigen::Triplet<double>>triplets to work
#include <vector>

Eigen::SparseMatrix<double> brusselator::buildA(const SimulationConfig& config)
{
	//Number of grid points in the x and y direction
	const int nx=config.nx;
	const int ny=config.ny;

	//total number of cells or number of unknowns
	const int N=nx*ny;
      
	// delta inverse squared
	const double alpha=1/(config.dx*config.dx);

	//Create an empty matrix A of type sparse
	
	Eigen::SparseMatrix<double> A(N,N);

	//Create a vector that stores all non zero entries
	
	std::vector<Eigen::Triplet<double>>triplets;

	/** Triplet is a vector that stores [row,col,value] */

    triplets.reserve(5*N);

	// Loop over every grid point
	
	for (int i=0; i<ny;++i)
	{
		for (int j=0;j<nx;++j)
		{
		// converting the local (i,j) into global index
			int k=i*nx+j;

			double diag=-4.0*alpha;

		
		//
		// GHOST CELL CONTRIBUTIONS AT THE BOUNDARIES
		//
		// Neumann BC:
		//
		// Cghost = Cboundary_value
		//
		// Every missing neighbour at the boundary
		// contributes +alpha to the diagonal.

		// Top boundary
			if (i==0)
				diag+=alpha;

		// Bottom boundary
			if (i==ny-1)
				diag+=alpha;

		// left boundary
			if (j==0)
				diag+=alpha;
		// right boundary
			if (j==nx-1)
				diag+=alpha;

		// Insert diagonal coefficient
			triplets.emplace_back(k,k,diag);

		//------------------------------------------
		// ACTUAL NEIGHBOURS
		// -----------------------------------------

		//------------------------------------------
		//North neighbour exists
		//------------------------------------------
	
			if (i>0)
            {
			
				int north=(i-1)*nx+j;
				triplets.emplace_back(k,north,alpha);
			}

		//------------------------------------------
		//South neighbour exists
		//------------------------------------------
	
			if (i<ny-1)
            {
				int south=(i+1)*nx+j;
				triplets.emplace_back(k,south,alpha);
			}

		//------------------------------------------
		//East neighbour exists
		//------------------------------------------

			if (j<nx-1)
            {
				int east=i*nx+(j+1);
				triplets.emplace_back(k,east,alpha);
            }


		//------------------------------------------
		//West neighbour exists
		//------------------------------------------
	
			if (j>0)
            {
			        int west=i*nx+(j-1);
                    triplets.emplace_back(k,west,alpha);
			}

		}
	}

	A.setFromTriplets(triplets.begin(),triplets.end());

	return A;

}	


Eigen::SparseMatrix<double> brusselator::buildAtilde(
        const Eigen::SparseMatrix<double>& A,
        const SimulationConfig& config)
{
    int N = config.nx * config.ny;

    //-------------------------------------------
    //Create a (2Nx2N) sparse matrix
    //-------------------------------------------

    Eigen::SparseMatrix<double> A_tilde(2*N,2*N);

    //-------------------------------------------
    //Store all non-zero entries
    //-------------------------------------------

    std::vector<Eigen::Triplet<double>> triplets;

    //-------------------------------------------
    //Upper-left block: D1*A
    //-------------------------------------------

    for (int k=0; k < A.outerSize(); ++k)
    {
        for (Eigen::SparseMatrix<double>::InnerIterator 
                it(A,k); it; ++it)
        {
            triplets.emplace_back(it.row(), it.col(),
                    config.D1 * it.value());
        }

    }

    //-------------------------------------------
    //Lower-right block: D2*A
    //-------------------------------------------

    for (int k=0; k < A.outerSize(); ++k)
    {
        for (Eigen::SparseMatrix<double>::InnerIterator
                it(A,k); it; ++it)
        {
            triplets.emplace_back(N+it.row(), N+it.col(),
                    config.D2 * it.value());
        }
    }

    //-------------------------------------------
    //Build sparse matrix
    //-------------------------------------------

    A_tilde.setFromTriplets(
            triplets.begin(),
            triplets.end());

    return A_tilde;

}




Eigen::VectorXd brusselator::initializeState(
        const SimulationConfig& config)
{
    //----------------------------------------
    //Number of cells
    //----------------------------------------

    const int N= config.nx * config.ny;

    //----------------------------------------
    //Initialize C1 & C2 uniformly
    //----------------------------------------

    Eigen::VectorXd C1= Eigen::VectorXd::Constant(
            N, config.Ca);

    Eigen::VectorXd C2= Eigen::VectorXd::Constant(N,
            config.Cb / config.Ca);

    //----------------------------------------
    //Add perturbations
    //----------------------------------------

    // Increase concentration of C1 by 20% at cell (10,10)
    // and decrease the concentration by 10% at cell (30,30)
    // k comes as a result of flattening the grid

    int k1= 10*config.nx + 10;

    int k2= 30*config.nx + 30;

    C1(k1)*=1.20;
    C1(k2)*=0.90;
    

    //-----------------------------------------

    // Increase the concentration of C2 by 20% at cell (10,10)
    // and decrease the concentration by 10% at cell (30,30)

    C2(k1)*=1.20;
    C2(k2)*=0.90;

    //------------------------------------------
    //Create combined state vector
    //------------------------------------------

    Eigen::VectorXd C_tilde(2*N);

    //------------------------------------------
    //First N entries: C1
    //------------------------------------------

    C_tilde.head(N)= C1;

    //------------------------------------------
    //Last N entries of C2
    //------------------------------------------

    C_tilde.tail(N)=C2;

    return C_tilde;

}


Eigen::VectorXd brusselator::computeReaction(const Eigen::VectorXd& C, const SimulationConfig& config)
{
    const int N = config.nx * config.ny;
    //Allocate reaction vector
    Eigen::VectorXd reaction= Eigen::VectorXd::Zero(2*N);

    //Return immediately if reactions are disabled
    
    if (!config.reactionEnabled)
    {
        return reaction;
    }

    //Compute Brusselator source terms
    
    for (int i=0;i<N; i++)
    {
        double C1=C(i);
        double C2=C(i+N);

        reaction(i)=config.Ca+C1*C1*C2-config.Cb*C1-C1;

        reaction(i+N)=config.Cb*C1-C1*C1*C2;
    }

    return reaction;

}



Eigen::VectorXd brusselator::odeSystem(
        const Eigen::VectorXd& C_tilde,
        const Eigen::SparseMatrix<double>& A_tilde,
        const SimulationConfig& config) 
{
    //---------------------------------------
    //Diffusion term
    //---------------------------------------

    Eigen::VectorXd diffusion= A_tilde * C_tilde;

    //---------------------------------------
    //Reaction term
    //---------------------------------------

    Eigen::VectorXd reaction= brusselator::computeReaction(C_tilde, config);



    return diffusion+reaction;

}


Eigen::VectorXd brusselator::computeResidual(const Eigen::VectorXd& Cguess, const Eigen::VectorXd& Cold, const SimulationConfig& config)
{
    //Evaluate the rhs which is the derivative of the 
    //governing equation, that includes both diffusion and reaction
    Eigen::VectorXd rhs=
        brusselator::odeSystem(Cguess,config.A_tilde,config);

    // Compute the backward euler residual

    Eigen::VectorXd residual=
        Cguess - Cold - config.dt*rhs;

    return residual;

}

Eigen::SparseMatrix<double> brusselator::assembleJacobian(const Eigen::VectorXd& Cguess, const SimulationConfig& config)
{
    const int N = config.nx * config.ny;
    //-------------------------------------------------
    //Assembling of reaction Jacobian Jw
    //-------------------------------------------------

    std::vector<Eigen::Triplet<double>> triplets;

    for (int cell=0; cell<N; cell++)
    {
        //global indices
        int c1=cell;
        int c2=cell+N;

        double C1=Cguess(c1);
        double C2=Cguess(c2);

        //---------------------------------------------
        //Local reaction Jacobian entries
        //---------------------------------------------

        double dw1_dC1= -(config.Cb+1.0)+ 2.0 * C1* C2;
        double dw1_dC2= C1*C1;

        double dw2_dC1= config.Cb - 2.0*C1*C2;
        double dw2_dC2= -C1*C1;

        //---------------------------------------------
        //Insert the 2x2 local block for one cell
        //---------------------------------------------

        triplets.emplace_back(c1,c1,dw1_dC1);
        triplets.emplace_back(c1,c2,dw1_dC2);

        triplets.emplace_back(c2,c1,dw2_dC1);
        triplets.emplace_back(c2,c2,dw2_dC2);
    }

    Eigen::SparseMatrix<double> Jw(2*N, 2*N);
    Jw.setFromTriplets(triplets.begin(),triplets.end());

    //----------------------------------------------------
    //Identity matrix
    //----------------------------------------------------

    Eigen::SparseMatrix<double> I(2*N,2*N);
    I.setIdentity();

    //----------------------------------------------------
    //with reaction OFF
    //----------------------------------------------------
    
    if (!config.reactionEnabled)
    {
        Eigen::SparseMatrix<double> I(2*N,2*N);
        I.setIdentity();

        return I-config.dt*config.A_tilde;
    }

    //with reaction ON
    else
    {
        return I-config.dt*(config.A_tilde+ Jw);
    }

}
