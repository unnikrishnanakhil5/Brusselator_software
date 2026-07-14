#pragma once
#include <Eigen/Dense>
#include "simconfig.hpp"
#include <sstream>
#include <iomanip>
#include <fstream>
void writeVTK(const Eigen::VectorXd& C,const SimulationConfig& config, int timestep); 


void appendPVDentry(int timestep, double time);
