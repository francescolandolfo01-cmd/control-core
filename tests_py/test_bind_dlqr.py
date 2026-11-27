import numpy as np
import pytest

from ccref.discretize import zoh
from ccref.models import cart_pole_upright
from ccref.lqr import dlqr

# Se il modulo C++ non è compilato, i test vengono saltati invece di fallire.
cc = pytest.importorskip("ccref._cc")


def cart_pole_problem():
    # d = zoh(cart_pole_upright(), 0.02); Q = diag(1, 1, 10, 1); R = [[0.1]]
    d=zoh(cart_pole_upright(), 0.02)
    Q=np.diag([1, 1, 10, 1])
    R = np.array([[0.1]])
    # return d, Q, R
    return d, Q, R


def test_binding_matches_ccref():
    d, Q, R = cart_pole_problem()
    ok, P, K = cc.dlqr_cart_pole(d.A, d.B, Q, R)
    # P_ref, K_ref = dlqr Python
    P_ref, K_ref = dlqr(d.A, d.B, Q, R)
    # assert ok
    assert ok
    # K contro K_ref: rtol=0, atol=1e-10
    # P contro P_ref: rtol=0, atol=1e-8
    np.testing.assert_allclose(K.reshape(1, 4), K_ref, rtol=0, atol=1e-10)
    np.testing.assert_allclose(P, P_ref, rtol=0, atol=1e-8)

def test_wrong_shape_raises():
    # con pytest.raises(TypeError): binding con d.A[:3, :3] al posto di d.A
    d, Q, R = cart_pole_problem()
    with pytest.raises(TypeError):
        cc.dlqr_cart_pole(d.A[:3, :3], d.B, Q, R)

def test_not_converged():
    # binding con max_iter=10 → ok deve essere False
    d, Q, R = cart_pole_problem()
    ok, _, _ = cc.dlqr_cart_pole(d.A, d.B, Q, R, max_iter=10)
    assert not ok
