import sys
import numpy as np
import pyvista as pv

# -----------------------------
# Read files
# -----------------------------
reference = pv.read("../resources/final_output_30s.vts")

exp = pv.read("../results/Explicit_Euler/output_3000.vti")
rk4 = pv.read("../results/Runge-Kutta/output_3000.vti")
imp = pv.read("../results/Implicit_Euler/output_3000.vti")

# -----------------------------
# Extract arrays
# -----------------------------
ref_C1 = reference["C1"]
ref_C2 = reference["C2"]

exp_C1 = exp["C1"]
exp_C2 = exp["C2"]

rk4_C1 = rk4["C1"]
rk4_C2 = rk4["C2"]

imp_C1 = imp["C1"]
imp_C2 = imp["C2"]

# -----------------------------
# Error metrics
# -----------------------------

# L2 norm
def L2(a, b):
    return np.sqrt(np.mean((a - b) ** 2))

# Maximum Error
def Linf(a, b):
    return np.max(np.abs(a - b))

# Mean Absolute Error

def MAE(a, b):
    return np.mean(np.abs(a,b))

# -----------------------------
# Explicit Euler
# -----------------------------
exp_L2_C1 = L2(exp_C1, ref_C1)
exp_Linf_C1 = Linf(exp_C1, ref_C1)
exp_MAE_C1 = MAE(exp_C1, ref_C1)

exp_L2_C2 = L2(exp_C2, ref_C2)
exp_Linf_C2 = Linf(exp_C2, ref_C2)
exp_MAE_C2 = MAE(exp_C2, ref_C2)

# -----------------------------
# RK4
# -----------------------------
rk4_L2_C1 = L2(rk4_C1, ref_C1)
rk4_Linf_C1 = Linf(rk4_C1, ref_C1)
rk4_MAE_C1 = MAE(rk4_C1, ref_C1)

rk4_L2_C2 = L2(rk4_C2, ref_C2)
rk4_Linf_C2 = Linf(rk4_C2, ref_C2)
rk4_MAE_C2 = MAE(rk4_C2, ref_C2)

# -----------------------------
# Implicit Euler
# -----------------------------
imp_L2_C1 = L2(imp_C1, ref_C1)
imp_Linf_C1 = Linf(imp_C1, ref_C1)
imp_MAE_C1 = MAE(imp_C1, ref_C1)

imp_L2_C2 = L2(imp_C2, ref_C2)
imp_Linf_C2 = Linf(imp_C2, ref_C2)
imp_MAE_C2 = MAE(imp_C2, ref_C2)

# -----------------------------
# Tolerances
# -----------------------------
TOL_L2 = 0.42
TOL_LINF = 2.9
TOL_MAE = 3.2

passed = (
    exp_L2_C1 < TOL_L2 and
    exp_Linf_C1 < TOL_LINF and
    exp_MAE_C1 < TOL_MAE and

    exp_L2_C2 < TOL_L2 and
    exp_Linf_C2 < TOL_LINF and
    exp_MAE_C2 < TOL_MAE and

    rk4_L2_C1 < TOL_L2 and
    rk4_Linf_C1 < TOL_LINF and
    rk4_MAE_C1 < TOL_MAE and

    rk4_L2_C2 < TOL_L2 and
    rk4_Linf_C2 < TOL_LINF and
    rk4_MAE_C2 < TOL_MAE and

    imp_L2_C1 < TOL_L2 and
    imp_Linf_C1 < TOL_LINF and
    imp_MAE_C1 < TOL_MAE and

    imp_L2_C2 < TOL_L2 and
    imp_Linf_C2 < TOL_LINF and
    imp_MAE_C2 < TOL_MAE
)

print("PASS" if passed else "FAIL")

sys.exit(0 if passed else 1)
