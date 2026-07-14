import argparse
import os

import numpy as np
import vtk
from vtk.util.numpy_support import vtk_to_numpy


def read_vtk(filename):
    """
    Read a VTK XML dataset (.vti or .vts).

    Parameters
    ----------
    filename : str
        Path to the VTK file.

    Returns
    -------
    vtkDataSet
        Loaded VTK dataset.
    """

    extension = os.path.splitext(filename)[1].lower()

    if extension == ".vti":
        reader = vtk.vtkXMLImageDataReader()

    elif extension == ".vts":
        reader = vtk.vtkXMLStructuredGridReader()

    else:
        raise ValueError(f"Unsupported file format: {extension}")

    reader.SetFileName(filename)
    reader.Update()

    dataset = reader.GetOutput()

    if dataset is None:
        raise RuntimeError(f"Failed to read {filename}")

    return dataset


def print_dataset_info(dataset, title):
    """
    Print basic information about a VTK dataset.
    """

    print("=" * 60)
    print(title)
    print("=" * 60)

    print(f"Dataset type      : {dataset.GetClassName()}")
    print(f"Number of points  : {dataset.GetNumberOfPoints()}")
    print(f"Number of cells   : {dataset.GetNumberOfCells()}")

    point_data = dataset.GetPointData()

    print(f"Point arrays      : {point_data.GetNumberOfArrays()}")

    print("\nAvailable arrays:")

    for i in range(point_data.GetNumberOfArrays()):
        print(f"  {i}: {point_data.GetArrayName(i)}")

    print()


def get_scalar(dataset, name):
    """
    Extract a scalar field from the dataset.

    Parameters
    ----------
    dataset : vtkDataSet
    name : str

    Returns
    -------
    numpy.ndarray
    """

    vtk_array = dataset.GetPointData().GetArray(name)

    if vtk_array is None:
        raise RuntimeError(f"Array '{name}' not found.")

    return vtk_to_numpy(vtk_array)


def compare_scalar(solution, reference, name):
    """
    Compare two scalar fields and print error statistics.
    """

    error = solution - reference

    abs_error = np.abs(error)

    max_error = np.max(abs_error)
    mean_error = np.mean(abs_error)
    rmse = np.sqrt(np.mean(error**2))
    l2_error = np.linalg.norm(error)
    relative_l2 = l2_error / np.linalg.norm(reference)

    print("=" * 60)
    print(name)
    print("=" * 60)

    print(f"Maximum error : {max_error:.6e}")
    print(f"Mean error    : {mean_error:.6e}")
    print(f"RMSE          : {rmse:.6e}")
    print(f"L2 error      : {l2_error:.6e}")
    print(f"Relative L2   : {relative_l2:.6e}")

    print()

    return relative_l2


def main():

    parser = argparse.ArgumentParser(
        description="Compare a solver solution against a reference solution."
    )

    parser.add_argument(
        "solution",
        help="Solver output (.vti)"
    )

    parser.add_argument(
        "reference",
        help="Reference solution (.vts)"
    )

    args = parser.parse_args()

    print("\nReading datasets...\n")

    solution = read_vtk(args.solution)
    reference = read_vtk(args.reference)

    print_dataset_info(solution, "My Solution")
    print_dataset_info(reference, "Reference Solution")

    if solution.GetNumberOfPoints() != reference.GetNumberOfPoints():
        raise RuntimeError("Number of points does not match.")

    if solution.GetNumberOfCells() != reference.GetNumberOfCells():
        raise RuntimeError("Number of cells does not match.")

    C1_solution = get_scalar(solution, "C1")
    C2_solution = get_scalar(solution, "C2")

    C1_reference = get_scalar(reference, "C1")
    C2_reference = get_scalar(reference, "C2")

    print("Comparing scalar fields...\n")

    rel_C1 = compare_scalar(C1_solution, C1_reference, "C1")
    rel_C2 = compare_scalar(C2_solution, C2_reference, "C2")

    tolerance = 4

    print("=" * 60)

    if rel_C1 < tolerance and rel_C2 < tolerance:
        print("Validation PASSED")
        return 0

    print("Validation FAILED")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
