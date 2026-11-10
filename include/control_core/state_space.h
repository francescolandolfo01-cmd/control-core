#ifndef CONTROL_CORE_STATE_SPACE_H
#define CONTROL_CORE_STATE_SPACE_H

#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>

namespace control_core
{
template <int Nx, int Nu>
class StateSpace
{
public:
    StateSpace(const Eigen::Matrix<double, Nx, Nx>& A,
               const Eigen::Matrix<double, Nx, Nu>& B,
               double Ts,
               const Eigen::Matrix<double, Nx, 1>& x0)
        : m_x{ x0 }
    {
        Eigen::Matrix<double, Nx + Nu, Nx + Nu> M = Eigen::Matrix<double, Nx + Nu, Nx + Nu>::Zero();
        M.block(0, 0, Nx, Nx) = A;
        M.block(0, Nx, Nx, Nu) = B;
        Eigen::Matrix<double, Nx + Nu, Nx + Nu> E = (M*Ts).exp();
        m_Ad = E.block(0, 0, Nx, Nx);
        m_Bd = E.block(0, Nx, Nx, Nu);
    }

    void step(const Eigen::Matrix<double, Nu, 1>& u) { m_x = m_Ad*m_x + m_Bd*u; }
    Eigen::Matrix<double, Nx, 1> state() const { return m_x; }
    Eigen::Matrix<double, Nx, Nx> Ad() const { return m_Ad; }
    Eigen::Matrix<double, Nx, Nu> Bd() const { return m_Bd; }

private:
    Eigen::Matrix<double, Nx, Nx> m_Ad{};
    Eigen::Matrix<double, Nx, Nu> m_Bd{};
    Eigen::Matrix<double, Nx, 1> m_x{};
};
}

#endif
