import numpy as np
from scipy.linalg import solve_discrete_are


def dlqr(Ad: np.ndarray, Bd: np.ndarray, Q: np.ndarray, R: np.ndarray) -> tuple[np.ndarray, np.ndarray]:
    # P = soluzione della DARE con solve_discrete_are(Ad, Bd, Q, R)
    P=solve_discrete_are(Ad, Bd, Q, R)
    # K = (R + Bdᵀ P Bd)⁻¹ Bdᵀ P Ad  → con np.linalg.solve, non inv
    K=np.linalg.solve(R + Bd.T @ P @ Bd, Bd.T @ P @ Ad)
    # return P, K
    return P, K
