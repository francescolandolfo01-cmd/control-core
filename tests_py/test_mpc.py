import numpy as np

from ccref.discretize import zoh
from ccref.models import cart_pole_upright
from ccref.lqr import dlqr
from ccref.mpc import prediction_matrices, condensed_cost, mpc_unconstrained, mpc_box_osqp

def _cart_pole_mpc():
    # d = zoh(cart_pole_upright(), 0.02); Q = diag(1, 1, 10, 1); R = [[0.1]]; P, K = dlqr(...)
    d = zoh(cart_pole_upright(), 0.02)
    Q = np.diag([1.0, 1.0, 10.0, 1.0])
    R = np.array([[0.1]])
    P, K = dlqr(d.A, d.B, Q, R)
    return d, Q, R, P, K


def test_prediction_matches_simulation():
    d, Q, R, P, K = _cart_pole_mpc()
    N = 20
    x0 = np.array([0.0, 0.0, 0.1, 0.0])
    U = np.linspace(-1.0, 1.0, N)
    Sx, Su = prediction_matrices(d.A, d.B, N)
    X = Sx @ x0 + Su @ U
    x = x0.copy()
    for k in range(N):
        x = d.A @ x + d.B[:, 0] * U[k]
        np.testing.assert_allclose(x, X[k*4:(k+1)*4], rtol=0, atol=1e-12)


def test_unconstrained_first_input_is_lqr():
    d, Q, R, P, K = _cart_pole_mpc()
    N = 20
    x0 = np.array([0.0, 0.0, 0.1, 0.0])
    U = np.linspace(-1.0, 1.0, N)
    Sx, Su = prediction_matrices(d.A, d.B, N)
    # stesso setup; Sx, Su → H, F → U = mpc_unconstrained(...)
    H, F = condensed_cost(Sx, Su, Q, R, P, N)
    U = mpc_unconstrained(H, F, x0)
    np.testing.assert_allclose(U[0], -(K @ x0)[0], rtol=0, atol=1e-10)

def test_osqp_matches_unconstrained_when_inactive():
    d, Q, R, P, K = _cart_pole_mpc()
    N = 20
    x0 = np.array([0.0, 0.0, 0.1, 0.0])
    Sx, Su = prediction_matrices(d.A, d.B, N)
    H, F = condensed_cost(Sx, Su, Q, R, P, N)
    U_ref = mpc_unconstrained(H, F, x0)
    U = mpc_box_osqp(H, F, x0, 10.0)
    np.testing.assert_allclose(U, U_ref, rtol=0, atol=1e-6)


def test_osqp_box_active():
    # stesso setup (5 righe), poi:
    d, Q, R, P, K = _cart_pole_mpc()
    N = 20
    x0 = np.array([0.0, 0.0, 0.1, 0.0])
    Sx, Su = prediction_matrices(d.A, d.B, N)
    H, F = condensed_cost(Sx, Su, Q, R, P, N)
    U = mpc_box_osqp(H, F, x0, 2.0)
    assert np.abs(U).max() <= 2.0 + 1e-6
    np.testing.assert_allclose(U[0], 2.0, rtol=0, atol=1e-6)
    
