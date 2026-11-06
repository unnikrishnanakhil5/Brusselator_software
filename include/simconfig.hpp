#pragma once

#include <Eigen/Sparse>
#include <string>

class SimulationConfig
{
	public:
	// Grid parameters
	int nx=40;
	int ny=40;
	int N=nx * ny;

	double dx=0.005;
	double dy=0.005;

	// Time parameters
	
	double dt=0.01; //used to be 0.01
	double endtime=30.0;//used to be 30
	double writeInterval= 1.0;

	//Brusselator parameters
	
	double Ca=1.0;
	double Cb=3.0;

	//Diffusion coefficients
	
	double D1=1e-5;
	double D2=1e-6;

    bool reactionEnabled= true;

    std::string outputDirectory = "../results";
    std::string outputDirectory1 = "../results/Implicit";

    Eigen::SparseMatrix<double> A;
    Eigen::SparseMatrix<double> A_tilde;

};
