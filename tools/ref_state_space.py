import numpy as np
from scipy.signal import cont2discrete

A = np.array([[0.0, 1.0], [-2.0, -3.0]])
B = np.array([[0.0], [1.0]])
C = np.eye(2)
D = np.zeros((2, 1))
Ts = 0.1

Ad, Bd, _, _, _ = cont2discrete((A, B, C, D), Ts, method="zoh")
np.set_printoptions(precision=17)
print("Ad =\n", Ad)
print("Bd =\n", Bd)
