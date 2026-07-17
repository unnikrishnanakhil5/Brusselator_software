# Scientific Computing Final Project

Finite Difference Solver for the Two-Dimensional Brusselator Reaction-Diffusion System

---

## Overview

This repository contains the implementation of a finite difference solver for the two-dimensional Brusselator reaction-diffusion system developed for the course **Programming Tools for Scientific Computing (PT4SC)** at **Technische Universität Darmstadt**.

The project includes:

- Explicit Euler solver
- Classical Runge-Kutta (RK4) solver
- Implicit Backward Euler solver
- Bash scripts for parameter variation
- Docker development environment
- GitLab CI/CD pipeline
- VTK output for visualization with ParaView

---

# Project Structure

```text
scientific_computing_project/
│
├── CMakeLists.txt
├── README.md
├── requirements.txt
├── startContainer.sh
│
├── configs/
│   ├── Dockerfile
│   ├── custom_bashrc
│   ├── jupyter_lab_config.py
│   ├── neovim_config
│   └── python.md
│
├── include/
│   ├── brusselator.hpp
│   ├── integrator.hpp
│   ├── output.hpp
│   └── simconfig.hpp
│
├── src/
│   ├── brusselator.cpp
│   ├── integrator.cpp
│   ├── output.cpp
│   └── solver.cpp
│
├── scripts/
│   ├── compare_solution.py
│   ├── solution.pvd
│   ├── varyCb.sh
│   └── varyDt.sh
│
├── resources/
│   └── final_output_30s.vts
│
└── results/
    ├── Explicit_Euler/
    ├── Runge-Kutta/
    ├── Implicit_Euler/
    ├── varyCb/
    │   ├── Cb_1.5/
    │   ├── Cb_2.0/
    │   ├── Cb_2.5/
    │   └── Cb_3.0/
    │
    ├── varydt/
    │   ├── dt_0.01/
    │   ├── dt_0.02/
    │   ├── dt_0.03/
    │   ├── dt_0.04/
    │   └── dt_0.05/
    │
    ├── validation.ipynb
    └── Comparison of all methods.png
```

---

# Building the Code

The project is intended to be built inside the provided Docker container.

## 1. Build the Docker image

From the project root directory,

```bash
docker build -f configs/Dockerfile -t scientific-computing .
```

## 2. Start the container

```bash
./startContainer.sh
```

## 3. Configure and build the project

Inside the container,

```bash
mkdir -p build
cd build

cmake ..
make -j
```

The executable will be generated inside the `build/` directory.

---

# Running a Single Simulation

From the `build` directory, execute

```bash
./brusselator \
<solver> \
<nx> \
<ny> \
<tEnd> \
<dt> \
<writeInterval> \
<Ca> \
<Cb>
```

Example using the Runge-Kutta solver

```bash
./brusselator \
rk4 \
40 \
40 \
30 \
0.01 \
1 \
1 \
3
```

Example using the Implicit Euler solver

```bash
./brusselator \
implicit \
40 \
40 \
5 \
0.05 \
1 \
1 \
3
```

where

| Parameter | Description |
|-----------|-------------|
| `nx` | Number of grid points in the x-direction |
| `ny` | Number of grid points in the y-direction |
| `tEnd` | Simulation end time |
| `dt` | Time step |
| `writeInterval` | Time interval between output files |
| `Ca` | Brusselator parameter |
| `Cb` | Brusselator parameter |
| `method` | `explicit`, `rk4`, or `implicit` |

---

# Running the Parameter Variation Scripts

Two Bash scripts are provided for automated parameter studies.

## Parameter variation of Cb

```bash
cd scripts

./varyCb.sh
```

The script performs simulations for

- Cb = 1.5
- Cb = 2.0
- Cb = 2.5
- Cb = 3.0

The results are stored inside

```text
results/varyCb/
```

---

## Parameter variation of the time step

```bash
cd scripts

./varyDt.sh
```

The script performs simulations for

- dt = 0.01
- dt = 0.02
- dt = 0.03
- dt = 0.04
- dt = 0.05

The results are stored inside

```text
results/varydt/
```

---

# Result Organization

Simulation outputs are organized inside the `results/` directory.

```text
results/
│
├── Explicit_Euler/
│
├── Runge-Kutta/
│
├── Implicit_Euler/
│
├── varyCb/
│   ├── Cb_1.5/
│   ├── Cb_2.0/
│   ├── Cb_2.5/
│   └── Cb_3.0/
│
├── varydt/
│   ├── dt_0.01/
│   ├── dt_0.02/
│   ├── dt_0.03/
│   ├── dt_0.04/
│   └── dt_0.05/
│
├── validation.ipynb
└── Comparison of all methods.png
```

- `Explicit_Euler/` contains simulation results generated using the Explicit Euler method.
- `Runge-Kutta/` contains results generated using the classical RK4 method.
- `Implicit_Euler/` contains results generated using the Implicit Backward Euler solver.
- `varyCb/` contains the results from the parameter study in which the Brusselator parameter **Cb** is varied.
- `varydt/` contains the results from the parameter study in which the timestep **dt** is varied.
- `validation.ipynb` is used for post-processing and validation.
- `Comparison of all methods.png` is an image that compares the 3 numerical methods with the reference solution.

---

# Visualization

Simulation results are written in VTK (`.vti`) format and can be visualized using ParaView from inside the container

To visualize the solution:

1. Open ParaView.
2. Open the generated `.vti` file or `scripts/solution.pvd`.
3. Click **Apply**.
4. Select the desired scalar field (`C1` or `C2`) from the **Color By** menu.
