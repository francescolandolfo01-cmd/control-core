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

# --- disturbo costante: d = -3, u = 0.5 ---
d = -3.0
u = 0.5
z = np.array([0.0, 0.0, d])   # stato vero [x, ẋ, d]
zh = np.zeros(3)              # stima del LESO
b0 = 2.0
Gamma = np.array([Ts**2/2, Ts, 0.0])

first = None
for k in range(1, 1001):
    y = z[0]                  # misura PRIMA del passo della pianta
    zh = Phi @ zh + Gamma * b0 * u + L * (y - zh[0])                     # Φ ẑ + Γ b0 u + L (y − ẑ[0])
    z = Phi @ z + Gamma * b0 * u                      # Φ z + Γ b0 u
    if first is None and abs(zh[2] - d) < 1e-3:
        first = k
print(first, np.abs(zh - z).max())
