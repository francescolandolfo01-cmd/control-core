#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "control_core/rk4.h"

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
