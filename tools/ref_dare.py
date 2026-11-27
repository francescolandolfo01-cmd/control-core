import numpy as np

from ccref.discretize import zoh
from ccref.models import cart_pole_upright
from ccref.lqr import dlqr

Ts=0.02
d=zoh(cart_pole_upright(), Ts)
Q = np.diag([1.0, 1.0, 10.0, 1.0])
R = np.array([[0.1]])
P, K= dlqr(d.A, d.B, Q, R)

np.set_printoptions(precision=17)

print("Ad =\n", d.A)
print("Bd =\n", d.B)
print("P =\n", P)
print("K =\n", K)
