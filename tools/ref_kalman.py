import numpy as np
from scipy.signal import cont2discrete
from scipy.linalg import solve_discrete_are

A = np.zeros((4,4))
A[0][1] = 1
A[1][2] = -0.981
A[2][3] = 1
A[3][2] = 21.582
B = np.array([[0.0], [1.0], [0.0], [-2.0]])

Ts = 0.02
C = np.array([[1.0, 0.0, 0.0, 0.0],
              [0.0, 0.0, 1.0, 0.0]])
D = np.zeros((2, 1))

Ad, Bd, _, _, _ = cont2discrete((A, B, C, D), Ts, method="zoh")
Qn = 1e-4 * np.eye(4)
Rn = 1e-3 * np.eye(2)

Pm = solve_discrete_are( Ad.T, C.T, Qn, Rn)   #   ← duale
S  = C @ Pm @ C.T + Rn
L  = ( np.linalg.solve(S, C @ Pm) ).T
Pp = (np.eye(4) - L @ C) @ Pm
# stampa con precision=17: L (4x2) e np.diag(Pp)
np.set_printoptions(precision=17)
print("L =\n", L)
print("diag(Pp) =", np.diag(Pp))

print(np.linalg.matrix_rank(np.hstack([C.T, A @ C.T, A @ A @ C.T, A @ A @ A @ C.T])))

