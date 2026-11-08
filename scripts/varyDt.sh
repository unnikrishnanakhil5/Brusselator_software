#!/bin/bash

# Check whether sufficient arguments are provided
if [ "$#" -lt 3 ]; then
    echo "Usage: ./varyDt.sh <variationName> <parameter> <values...>"
    echo "Example: ./varyDt.sh Variation_dt dt 0.01 0.02 0.03 0.04 0.05"
    exit 1
fi

variationName=$1
parameter=$2

shift 2

values=("$@")

echo "Variation name: $variationName"
echo "Parameter     : $parameter"

# Check whether the parameter is supported
if [ "$parameter" != "dt" ]; then
    echo "Error: Unsupported parameter '$parameter'"
    echo "This script supports only dt variation."
    exit 1
fi

# Default configuration of the solver similar to that in config

solver="implicit"

nx=40
ny=40

endtime=5.0

dt=0.01

writeInterval=1

Ca=1

Cb=3


# Loop to go through the values which the user gave
for value in "${values[@]}"

# assign the user-provided parameter value
do 

    if [ "$parameter" = "dt" ]
    then
        dt=$value
    fi


# Generate start.sh
    cat > start.sh << EOF
#!/bin/bash

../build/brusselator $solver $nx $ny $endtime $dt $writeInterval $Ca $Cb
EOF

# change mode + make executable
    chmod +x start.sh

    echo "Generated start.sh:"
    cat start.sh


    echo
    echo "Running simulation..."
    # execute the simulation
    ./start.sh

    #make a folder for the current run
    runFolder="../results/${variationName}/${parameter}_${value}"
    mkdir -p "$runFolder"

    #move the generated start.sh file in the runFolder
    mv start.sh "$runFolder/"

    #move the .vti files from the simulation to the runFolder/
    mv ../results/Implicit_Euler/*.vti "$runFolder/"
    mv ../results/Implicit_Euler/newton_history.txt "$runFolder/"


# print the current simulation configuration
    echo
    echo "------------------------------"
    echo "Current Simulation"
    echo "------------------------------"

    echo "Solver     : $solver"
    echo "Grid       : ${nx}x${ny}"
    echo "End Time   : $endtime"
    echo "dt         : $dt"
    echo "Write Interval : $writeInterval"
    echo "Ca         : $Ca"
    echo "Cb         : $Cb"

done
