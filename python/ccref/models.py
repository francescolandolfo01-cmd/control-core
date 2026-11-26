from dataclasses import dataclass

import numpy as np


@dataclass(frozen=True)
class LinearSystem:
    A: np.ndarray    # (nx, nx)
    B: np.ndarray    # (nx, nu)
    C: np.ndarray    # (ny, nx)
    D: np.ndarray    # (ny, nu)
    Ts: float = 0.0  # 0.0 = tempo continuo

    def __post_init__(self) -> None:
        nx = self.A.shape[0]
        nu = self.B.shape[1]
        ny = self.C.shape[0]
        if self.A.shape != (nx, nx):
            raise ValueError("A deve essere (nx, nx)")
        if self.B.shape[0] != nx:
            raise ValueError("B deve avere nx righe")
        if self.C.shape[1] != nx:
            raise ValueError("C deve avere nx colonne")
        if self.D.shape != (ny, nu):
            raise ValueError("D deve essere (ny, nu)")


def cart_pole_upright() -> LinearSystem:
    A = np.zeros((4, 4))
    A[0, 1] = 1.0
    A[1, 2] = -0.981
    A[2, 3] = 1.0
    A[3, 2] = 21.582
    B = np.array([[0.0], [1.0], [0.0], [-2.0]])
    C = np.eye(4)
    D = np.zeros((4, 1))
    return LinearSystem(A, B, C, D)
