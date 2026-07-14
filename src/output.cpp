#include "output.hpp"
#include <vtkImageData.h> //represents the computational grid
#include <vtkXMLImageDataWriter.h>
//This writes the grid to a file ending in .vti which paraview
//understands

#include <vtkSmartPointer.h>
//This prevents memory leaks

#include <vtkDoubleArray.h>
//to use vtkDoubleArray
#include <vtkPointData.h>
//to use GetPointData()

void writeVTK(const Eigen::VectorXd& C,const SimulationConfig& config, int timestep)
{
    auto grid= vtkSmartPointer<vtkImageData>::New();
    //New() creates an object of type vtkImageData somewhere 
    //inside heap, and grid is a vtkSmartPointer which points
    //to this object

    grid->SetDimensions(config.nx,config.ny,1); //since the problem is 2D
    // we use z=1 & 10 points in x and y directions

    grid->SetSpacing(config.dx,config.dy,1);

    //Create vtkDoubleArray. Just an empty table that stores
    //the values of concentrations
    auto C1array= vtkSmartPointer<vtkDoubleArray>::New();

    auto C2array= vtkSmartPointer<vtkDoubleArray>::New();

    //a name is given to the array inside the vtk file
    C1array->SetName("C1");

    C2array->SetName("C2");

    //tell vtk how many values to expect
    int N= config.nx * config.ny;

    C1array->SetNumberOfTuples(N);
    //allocating memory for the N concentration values

    //for loop for copying the values from C to C1array

    for(int k=0; k<N;k++)
    {
        C1array->SetValue(k,C(k));
    }

    //Attach the array to the mesh
    grid->GetPointData()->AddArray(C1array);
    grid->GetPointData()->SetActiveScalars("C1");

    C2array->SetNumberOfTuples(N);

    for(int l=0;l<N;l++)
    {
        C2array->SetValue(l,C(N+l));
    }

    grid->GetPointData()->AddArray(C2array);
    grid->GetPointData()->SetActiveScalars("C2");

    std::ostringstream filename; //empty string
    //append pieces to it
    filename << "output_";

    //append timestep

    //format it with leading zeros
    filename << std::setw(4) << std::setfill('0') << timestep;
    // std::setw(4) means to use atleast 4 characters
    // std::setfill('0') is used so that if the number 
    // is shorter than 4 characters, fill the remaíning places
    // with zeros

    filename << ".vti";

    auto writer=vtkSmartPointer<vtkXMLImageDataWriter>::New();

    writer->SetFileName(filename.str().c_str());
    //filename.str() is returns a std::string, .c_str() 
    //converts that into the C-style string (const char*) that
    //VTK expects

    writer->SetInputData(grid);

    writer->Write(); //actually creates the file on disk

}

void appendPVDentry(int timestep,double time)
{
    //open solution.pvd in append mode
    std::ofstream pvd("solution.pvd", std::ios::app);

    //Build the VTK filename
    std::ostringstream filename;

    filename <<"output_" << std::setw(4) << std::setfill('0')
        << timestep << ".vti";

    // Write one DataSet entry
    pvd << "  <DataSet " << "timestep=\"" << time << "\" "
        << "file=\"" << filename.str() << "\"/>\n";

    
}



