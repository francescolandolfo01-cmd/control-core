import numpy as np

from ccref.discretize import zoh
from ccref.models import cart_pole_upright
from ccref.lqr import dlqr
from ccref.leso import leso_gain
from ccref.leso import leso_gain_ackermann

def test_dlqr_matches_cpp():
    d = zoh(cart_pole_upright(), 0.02)
    Q = np.diag([1.0, 1.0, 10.0, 1.0])
    R = np.array([[0.1]])
    P, K = dlqr(d.A, d.B, Q, R)
    K_ref = np.array([[-2.719429743878661, -4.9479141334085375, -41.60979870360231, -9.375339783536937]])
    np.testing.assert_allclose(K, K_ref, rtol=0, atol=1e-10)

def test_dare_residual():
    d = zoh(cart_pole_upright(), 0.02)
    Q = np.diag(np.array([1.0, 1.0, 10.0, 1.0]))
    R = np.array([[0.1]])
    d = zoh(cart_pole_upright(), 0.02)
    P, K = dlqr(d.A, d.B, Q, R)
    res = Q + d.A.T @ P @ d.A - d.A.T @ P @ d.B @ np.linalg.solve(R + d.B.T @ P @ d.B, d.B.T @ P @ d.A) - P

    assert np.abs(res).max() < 1e-9

def test_closed_loop_stable():
    d = zoh(cart_pole_upright(), 0.02)
    Q = np.diag(np.array([1.0, 1.0, 10.0, 1.0]))
    R = np.array([[0.1]])
    d = zoh(cart_pole_upright(), 0.02)
    P, K = dlqr(d.A, d.B, Q, R)

    assert np.abs(np.linalg.eigvals(d.A - d.B @ K)).max() < 1

def test_leso_closed_form_matches_ackermann():
    L_formula = leso_gain(50.0, 1e-3)
    L_ack = leso_gain_ackermann(50.0, 1e-3)

    np.testing.assert_allclose(L_formula, L_ack, rtol=0, atol=1e-10)
    assert abs(L_ack[0] - 0.14631172649785792) < 1e-12

def test_leso_continuous_limit():
    L = leso_gain(50.0, 1e-6)
    np.testing.assert_allclose(L / 1e-6, [150.0, 7500.0, 125000.0], rtol=1e-4, atol=0)

