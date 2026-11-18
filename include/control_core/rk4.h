#include <Eigen/Dense>

namespace control_core
{
template <int Nx, int Nu, typename F>
Eigen::Matrix<double, Nx, 1> rk4Step(F f,
                                     const Eigen::Matrix<double, Nx, 1>& x,
                                     const Eigen::Matrix<double, Nu, 1>& u,
                                     double h)
	{

// f(x, u) = ẋ, con u costante sul passo (ZOH)
	Eigen::Matrix<double, Nx, 1> k1 = f(x, u);
	Eigen::Matrix<double, Nx, 1> k2 = f(x + h*k1/2, u);
	Eigen::Matrix<double, Nx, 1> k3 = f(x + h*k2/2, u);
	Eigen::Matrix<double, Nx, 1> k4 = f(x + h*k3, u);
// k1 = f(x, u)
// k2 = f(x + h/2·k1, u)
// k3 = f(x + h/2·k2, u)
// k4 = f(x + h·k3, u)
	return x + (k1 + 2*k2 + 2*k3 + k4)*h/6;
// x⁺ = x + h/6·(k1 + 2k2 + 2k3 + k4)
	}
}
