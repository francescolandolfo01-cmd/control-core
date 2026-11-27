#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <control_core/dlqr.h>

namespace py = pybind11;

// Istanza 4x1 (cart-pole) del template dlqr<Nx, Nu>.
// Restituisce la tupla Python (converged, P, K).
py::tuple dlqrCartPole(const Eigen::Matrix<double, 4, 4>& Ad,
                       const Eigen::Matrix<double, 4, 1>& Bd,
                       const Eigen::Matrix<double, 4, 4>& Q,
                       const Eigen::Matrix<double, 1, 1>& R,
                       int maxIter, double tol)
{
    // dichiara P (4x4) e K (1x4)
    Eigen::Matrix<double, 4, 4> P;
    Eigen::Matrix<double, 1, 4> K;
    bool ok = control_core::dlqr<4, 1>(Ad, Bd, Q, R, maxIter, tol, P, K);
    // ok = control_core::dlqr<4, 1>(…)   ← stessi argomenti di test_dlqr.cpp
    return py::make_tuple(ok, P, K);
    // return py::make_tuple()
}

PYBIND11_MODULE(_cc, m)
{
    m.doc() = "Pybind11 bindings dlqr";
    m.def("dlqr_cart_pole", &dlqrCartPole, "Function DLQR that calculates P and K", py::arg("Ad"), py::arg("Bd"), py::arg("Q"), py::arg("R"),
           py::arg("max_iter") = 100000, py::arg("tol") = 1e-12);
}
