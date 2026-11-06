cmake_minimum_required(VERSION 3.26)
project(brusselator
	VERSION 0.1
	LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Optimization and debug flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -g3")

# Setting the installation path
# set(CMAKE_INSTALL_PREFIX "/workspace/install")

#Add external libraries
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(VTK REQUIRED
   COMPONENTS
   CommonCore
   CommonDataModel
   IOXML
)

#Include directories for libraries 
include_directories(include)

#Add source files
set(SOURCES
  src/brusselator.cpp
  src/integrator.cpp
  src/solver.cpp
  src/output.cpp
)

#Add the executable
add_executable(brusselator ${SOURCES})

#Add external libraries
target_link_libraries(brusselator PRIVATE Eigen3::Eigen)
target_link_libraries(brusselator PRIVATE VTK::CommonCore
   VTK::CommonDataModel
   VTK::IOXML
)

#vtk_module_autoinit(TARGETS brusselator MODULES ${VTK_LIBRARIES})


#Installation rule
install(TARGETS brusselator DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

# Enable testing
enable_testing()

# find GoogleTest package
find_package(GTest REQUIRED)

# Add source files for testing
set(SOURCES_TEST
    tests/unit/test_laplacian.cpp
    tests/unit/test_reaction.cpp
    tests/unit/test_jacobian.cpp
    src/brusselator.cpp
)

#Create unit test executable
add_executable(unit_tests ${SOURCES_TEST})

# Add external libraries required for testing
target_link_libraries(unit_tests
       	PRIVATE
	GTest::gtest_main
       	Eigen3::Eigen
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(unit_tests)

