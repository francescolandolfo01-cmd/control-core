#include <gtest/gtest.h>
#include <Eigen/Dense>
#include <cmath>
#include "control_core/rk4.h"
#include "control_core/state_space.h"

TEST(Rk4, ScalarDecayOneStep)
{
    // lambda: (const Eigen::Matrix<double, 1, 1>& x, const Eigen::Matrix<double, 1, 1>&) -> -x
    //         il secondo parametro (u) è senza nome: non serve, e con -Werror
    //         un parametro con nome ma non usato blocca la compilazione
    auto a{ 
	[](const Eigen::Matrix<double, 1, 1>& x, const Eigen::Matrix<double, 1, 1>&) -> Eigen::Matrix<double, 1, 1>
	{
		return -x;
	}
       };

    // x0 = 1, u = 0 (qualunque), h = 0.1
    Eigen::Matrix<double, 1, 1> x0{};
    x0 << 1.0;
    Eigen::Matrix<double, 1, 1> u{};
    u << 1.0;
    const double h = 0.1;
    Eigen::Matrix<double, 1, 1> x1{};

    x1 = control_core::rk4Step<1,1>(a, x0, u, h);
    // un passo con control_core::rk4Step<1, 1>(...)

    // atteso: 1 - h + h^2/2 - h^3/6 + h^4/24 = 0.9048375, tolleranza 1e-15
    // il risultato è una matrice 1x1: l'elemento si legge con (0)
    EXPECT_NEAR(x1(0), 0.9048375, 1e-15);
}

TEST(Rk4, FourthOrderConvergence)
{
    auto a{
        [](const Eigen::Matrix<double, 1, 1>& x, const Eigen::Matrix<double, 1, 1>&) -> Eigen::Matrix<double, 1, 1>
        {
                return -x;
        }
       };

    Eigen::Matrix<double, 1, 1> u{};
    u << 1.0;
    const double h1 = 0.1;
    const double h2 = 0.05;
    Eigen::Matrix<double, 1, 1> x1{1.0};
    Eigen::Matrix<double, 1, 1> x2{1.0};
    double e1{};
    double e2{};
    double rapporto{};
    for(int i{0}; i<10; ++i)
	{
		x1 = control_core::rk4Step<1,1>(a, x1, u, h1);
	}
    for(int j{0}; j<20; ++j)
        {
                x2 = control_core::rk4Step<1,1>(a, x2, u, h2);
        }
    e1 = std::abs(x1(0) - std::exp(-1));
    e2 = std::abs(x2(0) - std::exp(-1));
    rapporto = e1/e2;

    EXPECT_NEAR(rapporto, 16.0, 2.0);
}

TEST(Rk4, MatchesStateSpaceZoh)
{
    Eigen::Matrix<double, 2, 2> A{};
    Eigen::Matrix<double, 2, 1> B{};
    A << 0.0, 1.0,
        -2.0, -3.0;
    B << 0.0,
         1.0;

    auto a{
        [A, B](const Eigen::Matrix<double, 2, 1>& x, const Eigen::Matrix<double, 1, 1>& u) -> Eigen::Matrix<double, 2, 1>
        {
                return A*x + B*u;
        }
       };
    Eigen::Matrix<double, 2, 1> x0{};
    x0 << 0.0,
          0.0;
    Eigen::Matrix<double, 1, 1> u{};
    u << 1.0;
    Eigen::Matrix<double, 2, 1> xr{x0};

    for(int i{0}; i<100; ++i)
    {
    	xr = control_core::rk4Step<2, 1>(a, xr, u, 0.01);
    }

    control_core::StateSpace<2, 1> sys{ A, B, 0.1, x0 };
    for(int j{0}; j<10; ++j)
    {
        sys.step(u);
    }
    EXPECT_NEAR(xr(0), sys.state()(0), 1e-9);
    EXPECT_NEAR(xr(1), sys.state()(1), 1e-9);
}

