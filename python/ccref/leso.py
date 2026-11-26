import numpy as np


def leso_gain(omega0: float, Ts: float) -> np.ndarray:
    # formula chiusa, beta = exp(-omega0*Ts)
    beta= np.exp(-omega0*Ts)
    l1 = 3*(1-beta)
    l2 = (1-beta)**2*(5 + beta)/(2*Ts)
    l3 = (1-beta)**3/(Ts**2)
    # restituisce np.array([l1, l2, l3])
    return np.array([l1, l2, l3])


def leso_gain_ackermann(omega0: float, Ts: float) -> np.ndarray:
    # come in tools/ref_leso.py: Phi, C, O = [C; C Phi; C Phi²], M = Phi − beta I
    # L = M³ O⁻¹ [0, 0, 1]ᵀ  (con np.linalg.solve)
    beta= np.exp(-omega0*Ts)
    Phi = np.array([[1.0, Ts, Ts**2/2], [0.0, 1.0, Ts], [0.0, 0.0, 1.0]])                                  # 3x3 come su >
    C = np.array([[1.0, 0.0, 0.0]])
    O = np.vstack([C, C @ Phi, C @ Phi @ Phi])
    M = Phi - beta*np.eye(3)
    L = M @ M @ M @ np.linalg.solve(O, np.array([0.0, 0.0, 1.0]))

    return L
