import numpy as np
import osqp
from scipy import sparse


def prediction_matrices(A: np.ndarray, B: np.ndarray, N: int) -> tuple[np.ndarray, np.ndarray]:
    # Sx: (N*nx, nx), Su: (N*nx, N*nu)
    nx = A.shape[0]
    nu = B.shape[1]
    Sx = np.zeros((N*nx, nx))
    Su = np.zeros((N*nx, N*nu))
    for r in range(N):
        Sx[r*nx:(r+1)*nx, :] = np.linalg.matrix_power(A, r+1)
        for j in range(r + 1):
            Su[r*nx:(r+1)*nx, j*nu:(j+1)*nu] = np.linalg.matrix_power(A, r-j) @ B
    return Sx, Su
    # blocco riga r = r*nx:(r+1)*nx  ↔  x_{r+1};   blocco colonna j = j*nu:(j+1)*nu  ↔  u_j
    # Sx[r] = A^(r+1);   Su[r, j] = A^(r-j) B  per j <= r


def condensed_cost(Sx: np.ndarray, Su: np.ndarray, Q: np.ndarray, R: np.ndarray,
                   P: np.ndarray, N: int) -> tuple[np.ndarray, np.ndarray]:
    # Qbar = kron(I_N, Q), poi l'ultimo blocco nx×nx diventa P;   Rbar = kron(I_N, R)
    Qbar = np.kron(np.eye(N), Q)
    nx = Sx.shape[1]
    Qbar[(N-1)*nx:N*nx, (N-1)*nx:N*nx] = P
    Rbar = np.kron(np.eye(N), R)
    # H = 2 (Suᵀ Qbar Su + Rbar),   F = 2 Suᵀ Qbar Sx
    H = 2*(Su.T @ Qbar @ Su + Rbar)
    F = 2*Su.T @ Qbar @ Sx
    return H, F

def mpc_unconstrained(H: np.ndarray, F: np.ndarray, x0: np.ndarray) -> np.ndarray:
    # U* = −H⁻¹ F x0   (np.linalg.solve, non inv)
    U_st = -np.linalg.solve(H, F @ x0)
    return U_st

def mpc_box_osqp(H: np.ndarray, F: np.ndarray, x0: np.ndarray, u_max: float) -> np.ndarray:
    # n = righe di H
    n = H.shape[0]
    # P_qp = sparse.triu(sparse.csc_matrix(H), format="csc")   ← solo il triangolo superiore
    P_qp = sparse.triu(sparse.csc_matrix(H), format="csc")
    # q = F @ x0;  A_qp = sparse.identity(n, format="csc");  l = -u_max * np.ones(n);  u = u_max * np.ones(n)
    q = F @ x0
    A_qp = sparse.identity(n, format="csc")
    l = -u_max * np.ones(n)
    u = u_max * np.ones(n)
    # prob = osqp.OSQP()
    prob = osqp.OSQP()
    # prob.setup(P_qp, q, A_qp, l, u, eps_abs=1e-9, eps_rel=1e-9, polish=False, verbose=False)
    prob.setup(P_qp, q, A_qp, l, u, eps_abs=1e-9, eps_rel=1e-9, polishing=False, verbose=False)
    # res = prob.solve()
    res = prob.solve(raise_error=False)
    # se res.info.status != "solved": raise RuntimeError("OSQP: " + res.info.status)
    if res.info.status != "solved":
            raise RuntimeError("OSQP: " + res.info.status)
    # return res.x
    return res.x
