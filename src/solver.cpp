#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iomanip>
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

int main(int argc, char*argv[]) 

{
    const int rk4 = 1;
    const int implicit = 2;
    const int explicitEuler = 3;
    int method = rk4; // default

    if(argc > 1)
    {
        std::string solver = argv[1];

        if(solver=="rk4")
            method=rk4;
        else if(solver=="implicit")
            method=implicit;
        else if(solver=="explicit")
            method=explicitEuler;
        else
        {
            std::cerr << "Unknown solver.\n";
            return EXIT_FAILURE;
        }
    }

    if(argc > 2)
        config.nx = std::stoi(argv[2]);
    if(argc > 3)
    config.ny = std::stoi(argv[3]);

    if(argc > 4)
    config.endtime = std::stod(argv[4]);
    if(argc > 5)
    config.dt = std::stod(argv[5]);
    if(argc > 6)
    config.writeInterval = std::stod(argv[6]);

    if(argc > 7)
    config.Ca = std::stod(argv[7]);
    if(argc > 8)
    config.Cb = std::stod(argv[8]);

     // Validating mesh parameters
    if (config.nx!= config.ny || config.nx < 40 || config.ny < 40)
    {
        std::cerr << "ERROR: Invalid Mesh parameters!\n";
        std::cerr << "REQUIREMENTS:\n";
        std::cerr << "1. The number of cells in the x and y directions must be equal.\n";
        std::cerr << "2. The number of cells in the x and y directions must be atleast 40.\n";

        return EXIT_FAILURE;
    }

     // Validating time parameters
    if(config.endtime <= 0 || config.dt <=0 || config.writeInterval <=0)
    {
        std::cerr << "ERROR: Invalid time parameters!\n";
        std::cerr << "endtime, dt and writeInterval must be positive!\n";

        return EXIT_FAILURE;
    }

    if(config.writeInterval < config.dt)
    {
        std::cerr << "ERROR: Invalid write interval!\n";
        std::cerr << "writeInterval must be atleast greater than the timestep.\n";

        return EXIT_FAILURE;
    }



    //

    std::cout << "Command: ";

    for(int i=0; i< argc; i++)
    {
        std::cout << argv[i] << " ";
    }

    std::cout << "\n";
    std::cout << "=========================================\n";
    std::cout << "Simulation Configuration\n";
    std::cout << "=========================================\n";

    std::cout << "Solver        : ";

    if(method == rk4)
    {
        std::ostringstream CbLabel;

        CbLabel << std::fixed
                << std::setprecision(2)
                << config.Cb;
        // Create output directory for capturing Runge-Kutta results
        config.outputDirectory1 = "../results/Runge-Kutta/";

        std::filesystem::create_directories(config.outputDirectory1);

        //Create output directory based on Cb
        config.outputDirectory= "../results/varyCb/Cb_" + CbLabel.str() + "/";

        //Create directory if it does not exist
        std::filesystem::create_directories(config.outputDirectory);



        std::cout << "Classical Runge-Kutta solver (RK4)\n";

    }
    else if(method == implicit)
    {
        //format dt to two decimal places
        std::ostringstream dtLabel;

        dtLabel << std::fixed
                << std::setprecision(2)
                << config.dt;
        //Create output directory for capturing implicit results

        config.outputDirectory1 = "../results/Implicit_Euler/";

        std::filesystem::create_directories(config.outputDirectory1);

        //Create output directory based on dt

        config.outputDirectory= "../results/varyDt/dt_" + dtLabel.str() + "/";

        // create directory if it does not exist

        std::filesystem::create_directories(config.outputDirectory);


        std::cout << "Implicit Backward Euler\n";

        std::cout << "Parameter variation dt = "
                  << config.dt << "\n";

        std::cout << "Output directories: "
                  << config.outputDirectory1 << "\n"
                  << config.outputDirectory << "\n\n";
        //config.outputDirectory = "../results/Implicit_Euler/";
        //std::cout << "Implicit Backward Euler\n";
    }
    else if(method == explicitEuler)
    {
        config.outputDirectory = "../results/Explicit_Euler/";

        std::filesystem::create_directories(config.outputDirectory);
        std::cout << "Explicit Forward Euler Solver\n";
    }

    std::cout << "Grid            : " << config.nx
        << " x " << config.ny << "\n";

    std::cout << "End Time        : "
              << config.endtime 
              << "s\n";

    std::cout << "dt              : "
              << config.dt << "s\n";

    std::cout << "Write Interval  : "
              << config.writeInterval << " s\n";

    std::cout << "Ca              : "
              << config.Ca << "\n";

    std::cout << "Cb              : "
              << config.Cb << "\n";

    std::cout << "=========================================\n\n";

    //Create a sparse matrix A that stores the coefficients 
    //of the central differencing scheme for the spatial 
    //discretization and store it inside config 
    config.A=brusselator::buildA(config);

    //Build A_tilde matrix from A
    config.A_tilde=brusselator::buildAtilde(config.A,config);

    //Initialize the state vector C_tilde as C
    typ C= brusselator::initializeState(config);


    beginPVD();

    //Adding the file to write the residual history
    std::ofstream residualFile(config.outputDirectory +
            "newton_history.txt");

    // header for the newton.txt
    residualFile << "TimeStep\tNewtonIteration\tResidual\n";

    int nSteps =static_cast<int>(config.endtime /config.dt);

    int writeEvery =static_cast<int>(config.writeInterval /
        config.dt);

    for (int timestep=0;timestep<=nSteps;timestep++)
    {
        double t=timestep*config.dt;

        if(timestep % writeEvery == 0)
        {
            if(method == implicit)
            {
                //First output directory
                std::string originalDirectory = config.outputDirectory;

                config.outputDirectory = config.outputDirectory1;
                writeVTK(C, config, timestep);

                //Second output directory
                config.outputDirectory = originalDirectory;

                writeVTK(C, config, timestep);
            }
            else if(method == rk4)
            {
                 //First output directory
                std::string originalDirectory = config.outputDirectory;

                config.outputDirectory = config.outputDirectory1;
                writeVTK(C, config, timestep);

                //Second output directory
                config.outputDirectory = originalDirectory;

                writeVTK(C, config, timestep);


            }
            else
            {
                writeVTK(C, config, timestep);
            }

            appendPVDentry(timestep, t, config);
        }

        if (timestep < nSteps)
        {
            if(method == rk4)
            {
                C=integrate::rk4(f,C,config);
            }
            else if(method == implicit)
            {
                C=integrate::implicitEuler(C, config,timestep,
                        residualFile);
            }
            else if(method == explicitEuler)
            {
                C=integrate::Euler(f,C, config);
            }
        }
    }

    endPVD();

    return 0;

}
