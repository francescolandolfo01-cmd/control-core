import dataclasses

import numpy as np
import pytest
from scipy.signal import cont2discrete

from ccref.discretize import zoh
from ccref.models import LinearSystem


def test_zoh_matches_cont2discrete():
    # A = [0 1; -2 -3], B = [0; 1], C = I2, D = zeros((2, 1)), Ts = 0.1
    A = np.array([[0., 1.],[-2., -3.]])
    B = np.array([[0.0], [1.0]])
    C = np.eye(2)
    D = np.zeros((2, 1))
    Ts = 0.1
    d = zoh(LinearSystem(A,B,C,D), Ts)
    # d = zoh(LinearSystem(...), Ts)
    # Ad_ref, Bd_ref, _, _, _ = cont2discrete((A, B, C, D), Ts, method="zoh")
    Ad_ref, Bd_ref, _, _, _ = cont2discrete((A, B, C, D), Ts, method="zoh")
    # assert_allclose su Ad e su Bd con rtol=0, atol=1e-12
    np.testing.assert_allclose(d.A, Ad_ref, rtol=0, atol=1e-12)
    np.testing.assert_allclose(d.B, Bd_ref, rtol=0, atol=1e-12)

def test_zoh_sets_ts():
    A = np.array([[0., 1.],[-2., -3.]])
    B = np.array([[0.0], [1.0]])
    C = np.eye(2)
    D = np.zeros((2, 1))
    Ts = 0.1
    d = zoh(LinearSystem(A,B,C,D), Ts)
    # stesso sistema, zoh con Ts = 0.1 → assert d.Ts == 0.1
    np.testing.assert_allclose(d.Ts, Ts, rtol=0, atol=1e-12)


def test_bad_shape_raises():
    # A di forma (2, 3): LinearSystem(...) dentro "with pytest.raises(ValueError):"
    A = np.zeros((2, 3))
    B = np.array([[0.0], [1.0]])
    C = np.eye(2)
    D = np.zeros((2, 1))
    with pytest.raises(ValueError):
        LinearSystem(A, B, C, D)

def test_frozen():
    # s = LinearSystem(...) valido
    A = np.array([[0., 1.],[-2., -3.]])
    B = np.array([[0.0], [1.0]])
    C = np.eye(2)
    D = np.zeros((2, 1))

    s = LinearSystem(A, B, C, D)
    with pytest.raises(dataclasses.FrozenInstanceError):
        s.Ts = 1.0
    # "s.Ts = 1.0" dentro "with pytest.raises(dataclasses.FrozenInstanceError):"
    ...
