# Brusselator Reaction-Diffusion Solver

Finite Difference Solver for the Two-Dimensional Brusselator Reaction-Diffusion System

---

## Overview

This project implements a numerical solver for the two-dimensional Brusselator reaction-diffusion equations using the Finite Difference Method (FDM). The project was developed as the final project for the course **Programming Tools for Scientific Computing** at **TU Darmstadt**.

The solver supports both explicit and implicit time integration methods and writes simulation results in VTK format for visualization in ParaView.

The project also includes

- CMake build system
- Bash automation for parameter studies
- Docker environment
- GitLab CI/CD pipeline
- Sparse matrix implementation using Eigen

---

## Mathematical Model

The governing equations are

∂C₁/∂t = D₁∇²C₁ + ω₁(C₁,C₂)

∂C₂/∂t = D₂∇²C₂ + ω₂(C₁,C₂)


where

ω₁ = Ca + C₁²C₂ − CbC₁ − C₁

ω₂ = CbC₁ − C₁²C₂

Spatial derivatives are discretized using second-order central finite differences with zero-gradient (Neumann) boundary conditions.

Time integration methods include

- Classical Runge-Kutta (Explicit)
- Backward Euler (Implicit)
- Inexact Newton Method for nonlinear systems

Sparse matrices from the Eigen library are used for efficient computation.

---

## Project Structure

scientific_computing_project/

│
├── CMakeLists.txt
├── README.md
├── Dockerfile
├── .gitlab-ci.yml
│
├── include/
│   ├── brusselator.hpp
│   ├── integrator.hpp
│   ├── output.hpp
│   └── simconfig.hpp
│
├── src/
│   ├── solver.cpp
│   ├── brusselator.cpp
│   ├── integrator.cpp
│   └── output.cpp
│
├── scripts/
│   └── parameter_variation.sh
│
├── build/
│
└── output/
```

---

# Dependencies

The project requires

- C++17 compiler
- CMake (≥3.16)
- Eigen3
- VTK
- Git

Ubuntu installation

```bash
sudo apt update

sudo apt install \
build-essential \
cmake \
libeigen3-dev \
libvtk9-dev
```

---

# Building the Project

Clone the repository

```bash
git clone <repository-url>

cd scientific_computing_project
```

Create the build directory

```bash
mkdir build
cd build
```

Configure

```bash
cmake ..
```

Compile

```bash
make -j
```

Executable

```
build/brusselator
```

---

# Running a Single Simulation

General syntax

```bash
./brusselator \
<nx> \
<ny> \
<t_end> \
<dt> \
<write_interval> \
<Ca> \
<Cb> \
<method>
```

Example

```bash
./brusselator \
40 \
40 \
30 \
0.01 \
1 \
1 \
3 \
rk4
```

or

```bash
./brusselator \
40 \
40 \
30 \
0.05 \
1 \
1 \
3 \
implicit
```

where

| Parameter | Description |
|------------|-------------|
| nx | Number of grid points (x-direction) |
| ny | Number of grid points (y-direction) |
| t_end | Simulation end time |
| dt | Time step |
| write_interval | Output interval |
| Ca | Brusselator parameter |
| Cb | Brusselator parameter |
| method | rk4 or implicit |

---

# Output

Simulation results are written in VTK format.

Example

```
output/

output_0000.vti
output_0100.vti
output_0200.vti
...
```

These files can be visualized using ParaView.

---

# Parameter Variation Script

The project includes a Bash script for automated parameter studies.

Example

```bash
./parameter_variation.sh \
--parameter Cb \
--values 1.5 2.0 2.5 3.0
```

or

```bash
./parameter_variation.sh \
--parameter dt \
--values 0.01 0.02 0.03 0.04 0.05
```

For each simulation the script

- creates a start script
- runs the solver
- stores outputs
- creates a dedicated directory

Example

```
results/

Cb_1.5/
Cb_2.0/
Cb_2.5/
Cb_3.0/
```

---

# Numerical Methods

## Spatial Discretization

- Second-order central differences
- Uniform Cartesian grid
- Neumann boundary conditions

## Explicit Solver

- Classical Runge-Kutta (RK4)

## Implicit Solver

- Backward Euler
- Inexact Newton iterations
- Sparse Jacobian
- Eigen sparse linear solver

---

# Visualization

Simulation outputs are written as VTK ImageData files.

Open ParaView

```
File
→ Open
→ output_3000.vti
```

Color by

```
C1
```

or

```
C2
```

---

# Docker

Build image

```bash
docker build -t brusselator .
```

Run container

```bash
docker run brusselator
```

---

# CI/CD

The GitLab CI pipeline automatically

- builds the project
- executes CMake
- runs parameter variation tests
- compares the numerical solution against the reference solution

---

# Reference Simulation

The reference case supplied in the project description uses

| Parameter | Value |
|-----------|------:|
| nx | 40 |
| ny | 40 |
| dx | 0.005 m |
| dy | 0.005 m |
| t_end | 30 s |
| dt | 0.01 s |
| Ca | 1 |
| Cb | 3 |
| D₁ | 1×10⁻⁵ m²/s |
| D₂ | 1×10⁻⁶ m²/s |

---

# Author

**Akhil Unnikrishnan**

M.Sc. Aerospace Engineering

Technische Universität Darmstadt

---

# Course

Programming Tools for Scientific Computing

Summer Semester 2026

TU Darmstadt

---

# License

This project was developed for academic purposes as part of the Programming Tools for Scientific Computing course.
