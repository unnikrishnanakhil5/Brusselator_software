# Brusselator Reaction-Diffusion solver software

## 1. Project Overview

This project implements a numerical solver for the two-dimensional, time-dependent Brusselator reaction-diffusion system using C++.

The main objective is to investigate the numerical behaviour of different time-integration schemes and study the influence of simulation parameters on the resulting solution.

The following numerical methods have been implemented:

- **Explicit Euler**
- **Classical Runge-Kutta 4 (RK4)**
- **Implicit Backward Euler**, using Newton iterations to solve the resulting nonlinear system.

The solver uses finite differences for spatial discretisation and the Eigen library for matrix operations and sparse linear algebra. Simulation results are written in VTK format and can be visualised using ParaView.

The project also includes automated parameter variation scripts, unit tests, component tests and numerical validation against reference data.

---

## 2. Project Structure

The project is organised as follows:

```text
scientific_computing_project/
│
├── CMakeLists.txt
├── README.md
├── requirements.txt
├── startContainer.sh
│
├── build/                          # CMake build directory
│
├── configs/                        # Docker and development configurations
│   ├── Dockerfile
│   ├── custom_bashrc
│   ├── jupyter_lab_config.py
│   ├── neovim_config
│   └── python.md
│
├── include/                        # C++ header files
│   ├── brusselator.hpp
│   ├── integrator.hpp
│   ├── output.hpp
│   └── simconfig.hpp
│
├── src/                            # C++ source files
│   ├── brusselator.cpp
│   ├── integrator.cpp
│   ├── output.cpp
│   └── solver.cpp
│
├── resources/                      # Reference data
│   └── final_output_30s.vts
│
├── scripts/                        # Parameter variation scripts
│   ├── varyCb.sh
│   └── varyDt.sh
│
├── tests/                          # Automated testing
│   │
│   ├── unit/                       # Unit tests
│   │   ├── test_jacobian.cpp
│   │   ├── test_laplacian.cpp
│   │   └── test_reaction.cpp
│   │
│   └── component/                  # Component-level tests
│       ├── parameter_cb_rk4.ipynb
│       ├── parameter_dt_implicit.ipynb
│       └── test_component.py
│
├── validation/                     # Numerical validation
│   └── validation.ipynb
│
└── results/                        # Simulation results and output files
```

---

## 3. Environment Setup Using Docker

The project uses Docker to provide a consistent development environment with the required C++ libraries, Python packages and visualisation tools.

### 3.1 Build the Docker image

The Dockerfile is located inside the `configs` directory.

From the project root directory, execute:

```bash
docker build -t brusselator-image -f configs/Dockerfile .
```

This builds the Docker image named `brusselator-image`.

### 3.2 Start the Docker container

The project includes a shell script named `startContainer.sh` in the root directory.

First, make the script executable:

```bash
chmod +x startContainer.sh
```

Then start the container:

```bash
./startContainer.sh
```

The script starts the Docker container and mounts the project directory to `/workspace` inside the container.

It also forwards the display-related environment variables and mounts the required WSLg/X11 directories to support graphical applications such as ParaView.

The container hostname is configured as:

```text
mycfdsolver-container
```

You can verify this from inside the container using:

```bash
hostname
```

The project files are accessible inside the container at:

```bash
/workspace
```

**Important:** Run `startContainer.sh` from the project root directory because the script uses `$(pwd)` to determine the directory to mount.

---

## 4. Compile the C++ Solver

After starting the container, navigate to the project directory:

```bash
cd /workspace
```

Create and enter the build directory:

```bash
mkdir -p build
cd build
```

Configure the project using CMake:

```bash
cmake ..
```

Compile the project using GNU Make:

```bash
make -j$(nproc)
```

### What happens during compilation?

- `cmake ..` reads the `CMakeLists.txt` file located in the project root directory.
- CMake identifies the required dependencies, configures the compiler and generates the appropriate build files.
- `make -j$(nproc)` executes the generated build instructions and compiles the C++ source files.
- The `-j$(nproc)` option allows Make to use the available CPU cores to perform parallel compilation.

The build process generates two important executables:

```text
brusselator
unit_tests
```

- `brusselator` is the main numerical solver.
- `unit_tests` is the executable used to run the C++ unit tests.

**Important:** You do not need a separate compilation procedure for the unit tests. The same `cmake ..` and `make -j$(nproc)` commands build both executables, provided the unit-test target is configured in `CMakeLists.txt`.

---

## 5. Run the Numerical Solver

The main executable can be run directly from the build directory.

### 5.1 Command-line syntax

```bash
./brusselator <solver> <nx> <ny> <t_end> <dt> <write_interval> <Ca> <Cb>
```

The arguments are:

| Argument | Description |
|---|---|
| `solver` | Numerical method: `explicit`, `implicit` or `rk4` |
| `nx` | Number of grid points in the x-direction |
| `ny` | Number of grid points in the y-direction |
| `t_end` | Final simulation time |
| `dt` | Time-step size |
| `write_interval` | Interval for writing simulation output |
| `Ca` | First Brusselator reaction parameter |
| `Cb` | Second Brusselator reaction parameter |

### 5.2 Example commands

**Explicit Euler:**

```bash
./brusselator explicit 40 40 30 0.01 1 1 3
```

**Implicit Backward Euler:**

```bash
./brusselator implicit 40 40 30 0.01 1 1 3
```

**Runge-Kutta 4:**

```bash
./brusselator rk4 40 40 30 0.01 1 1 3
```

The solver generates VTK output files that can subsequently be visualised using ParaView.

---

## 6. Automated Parameter Variation Studies

Two Bash scripts are provided to automate parameter variation studies.

Both scripts are located inside the `scripts` directory:

- `varyCb.sh`
- `varyDt.sh`

These scripts automatically generate a `start.sh` file for each simulation, execute the solver with the corresponding parameters and organise the resulting output files into separate directories.

**Important:** Execute these scripts from inside the `scripts` directory because they use relative paths to access the build executable and results directory.

Navigate to the scripts directory:

```bash
cd /workspace/scripts
```

Make both scripts executable:

```bash
chmod +x varyCb.sh varyDt.sh
```

### 6.1 Variation of the reaction parameter Cb using RK4

The `varyCb.sh` script is used to investigate the influence of the reaction parameter `Cb` on the numerical solution.

The simulation uses the RK4 solver with the following default configuration:

| Parameter | Value |
|---|---:|
| Solver | RK4 |
| Grid size | 40 × 40 |
| End time | 30 |
| Time step | 0.01 |
| Write interval | 1 |
| Ca | 1 |
| Cb | Variable |

**Command syntax:**

```bash
./varyCb.sh <variationName> <parameter> <values...>
```

**Example:**

```bash
./varyCb.sh Variation_Cb Cb 1.5 2.0 2.5 3.0
```

This command performs four simulations, using the specified values of `Cb`.

For every simulation, the script:

1. Updates the value of `Cb`.
2. Generates a `start.sh` script containing the corresponding solver command.
3. Executes the simulation.
4. Creates a separate directory for the parameter value.
5. Moves the generated `start.sh` and VTK output files into that directory.

The resulting directory structure follows this format:

```text
results/
└── Variation_Cb/
    ├── Cb_1.5/
    ├── Cb_2.0/
    ├── Cb_2.5/
    └── Cb_3.0/
```

Each directory contains the corresponding simulation output and the generated `start.sh` script.

### 6.2 Variation of the time step dt using the Implicit Backward Euler scheme

The `varyDt.sh` script investigates the influence of the time-step size on the numerical solution obtained using the Implicit Backward Euler scheme.

The simulation uses the following default configuration:

| Parameter | Value |
|---|---:|
| Solver | Implicit Backward Euler |
| Grid size | 40 × 40 |
| End time | 5.0 |
| Time step | Variable |
| Write interval | 1 |
| Ca | 1 |
| Cb | 3 |

**Command syntax:**

```bash
./varyDt.sh <variationName> <parameter> <values...>
```

**Example:**

```bash
./varyDt.sh Variation_dt dt 0.01 0.02 0.03 0.04 0.05
```

This command performs five simulations with different time-step sizes.

For every simulation, the script:

1. Updates the time-step size.
2. Generates a `start.sh` script containing the corresponding solver command.
3. Executes the implicit solver.
4. Creates a separate directory for the selected time-step size.
5. Moves the generated VTK output files, `newton_history.txt` and `start.sh` into that directory.

The resulting directory structure follows this format:

```text
results/
└── Variation_dt/
    ├── dt_0.01/
    ├── dt_0.02/
    ├── dt_0.03/
    ├── dt_0.04/
    └── dt_0.05/
```

The `newton_history.txt` file contains information about the Newton iteration history for the corresponding implicit simulation.

### 6.3 Customising the parameter variations

Both scripts accept a user-defined variation name, parameter name and a list of parameter values.

For example:

```bash
./varyCb.sh My_Cb_Study Cb 2.0 2.5 3.0
```

The user can specify different values without modifying the script itself.

Similarly, the time-step variation can be performed by supplying different values of `dt`:

```bash
./varyDt.sh My_dt_Study dt 0.01 0.02 0.05
```

The scripts use predefined simulation settings for the remaining parameters. If other simulation settings need to be changed, the corresponding default values inside the scripts can be modified.

The `Cb` parameter can also be varied by directly executing the solver from the build directory. Likewise, `dt` can be varied for the implicit solver through the command line, subject to the prescribed limits for the simulation.

### 6.4 Important information about simulation output directories

The solver itself creates method-specific output directories when a simulation is executed. For example:

```text
results/
├── Runge-Kutta/
└── Implicit_Euler/
```

The Bash scripts additionally create directories to organise the results of individual parameter variations.

For example:

```text
results/
├── Runge-Kutta/
├── Implicit_Euler/
├── Variation_Cb/
│   ├── Cb_1.5/
│   └── Cb_2.0/
└── Variation_dt/
    ├── dt_0.01/
    └── dt_0.02/
```

These directories serve different purposes:

- The method-specific directories are created by the solver to store simulation output.
- The parameter-specific directories are created by the Bash scripts to organise the results of individual parameter studies.

After each simulation, the scripts move the generated output files from the solver's method-specific directory into the corresponding parameter-specific directory.

Therefore, the presence of both types of directories is expected and does not indicate an error.

---

## 7. Visualisation Using ParaView

The solver writes simulation results in VTK `.vti` format.

These files can be opened using ParaView, which is installed inside the Docker environment.

### 7.1 Launch ParaView

From the container terminal, execute:

```bash
paraview
```

This launches the ParaView graphical interface.

### 7.2 Open the simulation results

1. Open ParaView.
2. Select **File → Open**.
3. Navigate to the relevant simulation output directory.
4. Select the required `.vti` file.
5. Click **Apply** in the Properties panel.
6. Select the scalar variable of interest, such as `C1` or `C2`, under the **Color By** option.

If multiple time-step files are available, they can be loaded as a time series to investigate the temporal evolution of the solution.

The animation controls can then be used to move through the available time steps.

**Note:** ParaView is a separate visualisation application. It is not directly integrated into the C++ solver.

---

## 8. Python Environment, Testing and Validation

Python is used in this project for component-level testing, post-processing and numerical validation.

The required Python dependencies are listed in `requirements.txt`.

### 8.1 Create and activate the Python virtual environment

From the project root directory inside the Docker container:

```bash
cd /workspace
```

Create a virtual environment:

```bash
python3 -m venv .venv
```

Activate it:

```bash
source .venv/bin/activate
```

Once activated, the virtual environment name should appear in the terminal prompt.

### 8.2 Install the required Python packages

Upgrade pip:

```bash
python -m pip install --upgrade pip
```

Install the project dependencies:

```bash
pip install -r requirements.txt
```

To verify the installed packages:

```bash
pip list
```

The virtual environment can be deactivated using:

```bash
deactivate
```

**Important:** Activate the virtual environment before running the Python component tests or opening the Python notebooks. This ensures that the required packages are available.

### 8.3 Unit Tests

The project includes C++ unit tests to verify the individual numerical components of the solver.

The unit tests are located inside:

```text
tests/unit/
```

The following test files are included:

| Test file | Purpose |
|---|---|
| `test_laplacian.cpp` | Tests the Laplacian operator used in the spatial discretisation. |
| `test_reaction.cpp` | Tests the reaction terms of the Brusselator system. |
| `test_jacobian.cpp` | Tests the Jacobian matrix used by the implicit solver. |

#### Building and executing the unit tests

The unit tests are included in the CMake build configuration.

Therefore, the same commands used to compile the main Brusselator solver also compile the unit-test executable.

From the project root directory:

```bash
cd /workspace/build
```

Configure the project:

```bash
cmake ..
```

Build the executables:

```bash
make -j$(nproc)
```

After compilation, the build directory contains the following executables:

```text
brusselator
unit_tests
```

To execute the unit tests, run:

```bash
./unit_tests
```

This executes the C++ unit tests and displays the corresponding test results in the terminal.

**Note:** There is no need to compile each unit-test source file separately. The CMake configuration handles the compilation and linking of the unit tests into the `unit_tests` executable.

### 8.4 Component Tests

Component-level testing is performed using Python and `pytest`.

The component tests are located inside:

```text
tests/component/
```

The main Python test file is:

```text
test_component.py
```

This test executes the numerical solver and checks its behaviour at the component level.

#### Running the component tests

First, activate the Python virtual environment:

```bash
cd /workspace
source .venv/bin/activate
```

Then execute the following command from the project root directory:

```bash
pytest tests/component/test_component.py
```

Pytest discovers and executes the component tests defined in the Python test file.

The test requires the compiled solver executable to be available in the build directory.

#### Component parameter variation notebooks

The component directory also contains two Jupyter notebooks:

| Notebook | Purpose |
|---|---|
| `parameter_cb_rk4.ipynb` | Investigates the variation of the reaction parameter `Cb` using the RK4 solver. |
| `parameter_dt_implicit.ipynb` | Investigates the influence of the time-step size `dt` using the Implicit Backward Euler scheme. |

These notebooks can be opened and executed using Jupyter Notebook.

To launch Jupyter Notebook, first activate the Python virtual environment:

```bash
cd /workspace
source .venv/bin/activate
```

Then navigate to the component directory:

```bash
cd /workspace/tests/component
```

Start the Jupyter Notebook server:

```bash
jupyter notebook
```

After executing the command, Jupyter Notebook will start in the terminal and display a URL.

Copy the following address and paste it into your web browser:

```text
http://127.0.0.1:8888/tree
```

This opens the Jupyter Notebook interface in your browser.

You can then open either of the following notebooks:

- `parameter_cb_rk4.ipynb`
- `parameter_dt_implicit.ipynb`

**Note:** The Docker startup script forwards port `8888` from the container to the host, allowing the notebook interface to be accessed through the browser.

### 8.5 Numerical Validation

Numerical validation is performed by comparing the computed solution against the reference solution provided in the `resources` directory.

The reference data is:

```text
resources/final_output_30s.vts
```

The validation notebook is located at:

```text
validation/validation.ipynb
```

The notebook can be used to load the reference solution and the numerical results generated by the solver, compare the corresponding fields and assess the accuracy of the numerical solution.

#### Opening the validation notebook

First, activate the Python virtual environment:

```bash
cd /workspace
source .venv/bin/activate
```

Navigate to the validation directory:

```bash
cd /workspace/validation
```

Start Jupyter Notebook:

```bash
jupyter notebook
```

Once the Jupyter server starts, copy the following address and paste it into your browser:

```text
http://127.0.0.1:8888/tree
```

The Jupyter Notebook interface will open, allowing you to access:

```text
validation.ipynb
```

Open the notebook and execute the cells to perform the numerical validation.

**Important:** The same procedure applies to opening the component notebooks in `tests/component`. The only difference is the directory from which `jupyter notebook` is launched.

---

## 9. Results and Output Organisation

The solver generates simulation output files in VTK format (`.vti`), which can be used for post-processing and visualisation.

The main output directories are:

```text
results/
├── Explicit_Euler/
├── Runge-Kutta/
├── Implicit_Euler/
├── Variation_Cb/
└── Variation_dt/
```

The first three directories contain the results generated by the respective numerical schemes.

The `Variation_Cb` and `Variation_dt` directories contain the results of the corresponding parameter studies, organised into separate subdirectories for each parameter value.

For the implicit solver, the output may also include:

```text
newton_history.txt
```

This file provides information about the Newton iteration history during the nonlinear solution procedure.

---

## 10. Quick Reference

The following table summarises the most frequently used commands.

| Task | Command |
|---|---|
| Build Docker image | `docker build -t brusselator-image -f configs/Dockerfile .` |
| Start Docker container | `./startContainer.sh` |
| Navigate to project directory | `cd /workspace` |
| Configure CMake | `cmake ..` |
| Compile solver and unit tests | `make -j$(nproc)` |
| Run Explicit Euler | `./brusselator explicit 40 40 30 0.01 1 1 3` |
| Run Implicit Euler | `./brusselator implicit 40 40 30 0.01 1 1 3` |
| Run RK4 | `./brusselator rk4 40 40 30 0.01 1 1 3` |
| Execute unit tests | `./unit_tests` |
| Create Python virtual environment | `python3 -m venv .venv` |
| Activate virtual environment | `source .venv/bin/activate` |
| Install Python dependencies | `pip install -r requirements.txt` |
| Run component tests | `pytest tests/component/test_component.py` |
| Run Cb variation study | `./varyCb.sh Variation_Cb Cb 1.5 2.0 2.5 3.0` |
| Run dt variation study | `./varyDt.sh Variation_dt dt 0.01 0.02 0.03 0.04 0.05` |
| Launch ParaView | `paraview` |
| Launch validation notebook | `cd /workspace/validation && jupyter notebook` |
| Launch component notebooks | `cd /workspace/tests/component && jupyter notebook` |

---

## 11. Final Notes

- Always launch the Docker container from the project root directory.
- The C++ solver and unit tests can be compiled together using CMake and GNU Make.
- Execute the parameter variation scripts from inside the `scripts` directory.
- Activate the Python virtual environment before running `pytest` or launching Jupyter Notebook.
- The validation and component notebooks can be accessed through the Jupyter Notebook interface in a browser using the forwarded port `8888`.
- The simulation results are stored in the `results` directory and are accessible from the host machine because the project directory is mounted into the Docker container.
- Use ParaView to visualise the generated VTK files and investigate the spatial and temporal evolution of the numerical solution.
