import numpy as np
from scipy.linalg import expm

from ccref.models import LinearSystem


def zoh(sys: LinearSystem, Ts: float) -> LinearSystem:
    nx=sys.A.shape[0]
    nu=sys.B.shape[1]
    M=np.zeros((nx+nu,nx+nu))
    M[:nx, :nx] = sys.A
    M[:nx, nx:] = sys.B
    E = expm(M * Ts)
    Ad = E[:nx, :nx]
    Bd = E[:nx, nx:]

    return LinearSystem(Ad, Bd, sys.C, sys.D, Ts)
    # nx = righe di sys.A, nu = colonne di sys.B
    # M = zeros (nx+nu) x (nx+nu); M[:nx, :nx] = A; M[:nx, nx:] = B
    # E = expm(M * Ts)
    # Ad = E[:nx, :nx], Bd = E[:nx, nx:]
    # return LinearSystem(Ad, Bd, sys.C, sys.D, Ts)

