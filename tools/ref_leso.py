import numpy as np

w0, Ts = 50.0, 0.001
beta = np.exp(-w0*Ts)
Phi = np.array([[1.0, Ts, Ts**2/2], [0.0, 1.0, Ts], [0.0, 0.0, 1.0]])                                  # 3x3 come su carta
C = np.array([[1.0, 0.0, 0.0]])
O = np.vstack([C, C @ Phi, C @ Phi @ Phi])
M = Phi - beta*np.eye(3)
L = M @ M @ M @ np.linalg.solve(O, np.array([0.0, 0.0, 1.0]))

np.set_printoptions(precision=17)
print("beta =\n", beta)
print("L =\n", L)
# stampa beta e L con np.set_printoptions(precision=17)
L_formula = [3*(1-beta),((1-beta)**2*(5+beta))/(2*Ts),(1-beta)**3/(Ts**2)]
# poi L_formula con le tre formule chiuse e np.abs(L - L_formula).max()
diff = np.abs(L - L_formula).max()
print("L_formula =\n", L_formula)
print("diff =\n", diff)
