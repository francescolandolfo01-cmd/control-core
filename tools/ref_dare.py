import numpy as np
from scipy.signal import cont2discrete
from scipy.linalg import solve_discrete_are

A = np.zeros((4,4))
A[0][1] = 1
A[1][2] = -0.981
A[2][3] = 1
A[3][2] = 21.582
B = np.array([[0.0], [1.0], [0.0], [-2.0]])
C = np.eye(4)
D = np.zeros((4, 1))
Ts = 0.02
Q = np.diag([1.0, 1.0, 10.0, 1.0])
R = np.array([[0.1]])

Ad, Bd, _, _, _ = cont2discrete((A, B, C, D), Ts, method="zoh")
np.set_printoptions(precision=17)
P = solve_discrete_are(Ad, Bd, Q, R)
K = np.linalg.solve(R + Bd.T @ P @ Bd, Bd.T @ P @ Ad)

print("Ad =\n", Ad)
print("Bd =\n", Bd)
print("P =\n", P)
print("K =\n", K)
