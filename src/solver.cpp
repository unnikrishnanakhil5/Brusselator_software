#include <iostream>
#include <fstream>
#include "simconfig.hpp"
#include "integrator.hpp"
#include "brusselator.hpp"
#include "output.hpp"

using typ= Eigen::VectorXd;

//Create a type of class SimulationConfig named config
SimulationConfig config;

//function to construct the rhs of the governing equations

typ f(const typ& C)
{
	typ dCdt= brusselator::odeSystem(C,
			config.A_tilde,config);
	return dCdt;
}

void beginPVD()
{
    std::ofstream pvd("solution.pvd");

    pvd << "<?xml version=\"1.0\"?>\n";

    pvd << "<VTKFile type=\"Collection\" version=\"0.1\">\n";

    pvd << "  <Collection>\n";
}

void endPVD()
{
    std::ofstream pvd("solution.pvd", std::ios::app);

    pvd << " </Collection>\n";

    pvd << "</VTKFile>\n";
}

int main() 

{
    std::cout << "=======================================\n";
    std::cout << "Select Time Integrator Method\n";
    std::cout << "=======================================\n";
    std::cout << "1. Classical Runge-Kutta (RK4)\n";
    std::cout << "2. Implicit Backward Euler\n ";
    std::cout << "Choice: ";

    // create a return type int for main and call it method
    int method;

    const int rk4= 1;
    const int implicit= 2;
    std::cin >> method;


    // Validate the input from the user

    if (method !=1 && method !=2)
    {
        std::cerr << "Invalid choice.\n";
        return -1;
    }

    if(method == rk4)
    {
        std::cout << "\nUsing Classical Runge-Kutta solver ...\n\n";
    }
    else if(method == implicit)
    {
        std::cout << "\nUsing Implicit Backward Euler Solver ....\n\n";
    }

    //Create a sparse matrix A that stores the coefficients 
    //of the central differencing scheme for the spatial 
    //discretization and store it inside config 
    config.A=brusselator::buildA(config);

    //Build A_tilde matrix from A
    config.A_tilde=brusselator::buildAtilde(config.A,config);

    //Initialize the state vector C_tilde as C
    typ C= brusselator::initializeState(config);

    int timestep=0;

    double t=0.0;

    double nextWriteTime=0.0;

    beginPVD();

    while(t<=config.endtime)
    {
        if (t>= nextWriteTime)
        {
            writeVTK(C,config,timestep);

            appendPVDentry(timestep, t);

            nextWriteTime+= config.writeInterval;
        }

        timestep=timestep+1;

        if(method == rk4)
        {
            C=integrate::rk4(f,C,config);
        }

        else if(method == implicit)
        {
            C=integrate::implicitEuler(C,config);
        }

        

        t+=config.dt;
    }

    endPVD();

    return 0;



}
