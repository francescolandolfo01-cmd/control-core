#include <gtest/gtest.h> 
#include <Eigen/Dense>
#include <cmath>
#include "control_core/cart_pole.h"
#include "control_core/rk4.h"

double cartPoleEnergy(const Eigen::Matrix<double, 4, 1>& x, const control_core::CartPoleParams& p)
{
    return 0.5*(p.M+p.m)*x(1)*x(1) + p.m*p.l*x(1)*x(3)*std::cos(x(2)) + 0.5*p.m*p.l*p.l*x(3)*x(3) + p.m*p.g*p.l*std::cos(x(2));
    // formula sopra, con x(1) = pdot, x(2) = theta, x(3) = thetadot
}

TEST(CartPole, EquilibriumUpright)
{
    // parametri di default
    control_core::CartPoleParams p{};

    // stato [p, pdot, theta, thetadot]: prima dichiari, poi <<
    Eigen::Matrix<double, 4, 1> x{};
    x << 0.3, 0.0, 0.0, 0.0;

    // ingresso F = 0
    Eigen::Matrix<double, 1, 1> u{};
    u << 0.0;

    // derivata dello stato
    Eigen::Matrix<double, 4, 1> xdot{ control_core::cartPoleDynamics(x, u, p) };

    // le 4 componenti devono essere zero, tolleranza 1e-12
    EXPECT_NEAR(xdot(0), 0.0, 1e-12);
    EXPECT_NEAR(xdot(1), 0.0, 1e-12);
    EXPECT_NEAR(xdot(2), 0.0, 1e-12);
    EXPECT_NEAR(xdot(3), 0.0, 1e-12);
    // ... altre 3 righe per xdot(1), xdot(2), xdot(3)
}

TEST(CartPole, EquilibriumDown)
{
    // parametri di default
    control_core::CartPoleParams p{};

    // stato [p, pdot, theta, thetadot]: prima dichiari, poi <<
    Eigen::Matrix<double, 4, 1> x{};
    x << 0.3, 0.0, std::acos(-1.0), 0.0;

    // ingresso F = 0
    Eigen::Matrix<double, 1, 1> u{};
    u << 0.0;

    // derivata dello stato
    Eigen::Matrix<double, 4, 1> xdot{ control_core::cartPoleDynamics(x, u, p) };

    // le 4 componenti devono essere zero, tolleranza 1e-12
    EXPECT_NEAR(xdot(0), 0.0, 1e-12);
    EXPECT_NEAR(xdot(1), 0.0, 1e-12);
    EXPECT_NEAR(xdot(2), 0.0, 1e-12);
    EXPECT_NEAR(xdot(3), 0.0, 1e-12);
    // ... altre 3 righe per xdot(1), xdot(2), xdot(3)
}

TEST(CartPole, LinearizationMatchesFiniteDifference)
{
    control_core::CartPoleParams p{};
    const double eps{ 1e-6 };

    // A, B analitici: dichiara, poi chiama cartPoleLinearUpright
    // (escono tramite i riferimenti)
    Eigen::Matrix<double, 4, 4> A{ Eigen::Matrix<double, 4, 4>::Zero() };
    Eigen::Matrix<double, 4, 1> B{ Eigen::Matrix<double, 4, 1>::Zero() };
    control_core::cartPoleLinearUpright(p, A, B);

    // stato e ingresso di equilibrio: tutti zero, con ::Zero()
    // 2. punto di equilibrio
    Eigen::Matrix<double, 4, 1> x0{ Eigen::Matrix<double, 4, 1>::Zero() };
    Eigen::Matrix<double, 1, 1> u0{ Eigen::Matrix<double, 1, 1>::Zero() };

    // A numerica: un ciclo su j = 0..3
    //   xp = x0 con +eps in posizione j, xm = x0 con -eps in posizione j
    //   colonna j = (f(xp, u0) - f(xm, u0)) / (2 * eps)
    //   la colonna si scrive con .block(0, j, 4, 1) oppure .col(j)
    // 3. A numerica, una colonna per giro
    Eigen::Matrix<double, 4, 4> An{ Eigen::Matrix<double, 4, 4>::Zero() };
    for (int j{ 0 }; j < 4; ++j)
    {
        Eigen::Matrix<double, 4, 1> xp{ x0 };
        Eigen::Matrix<double, 4, 1> xm{ x0 };
        xp(j) = xp(j) + eps;
        xm(j) = xm(j) - eps;
        Eigen::Matrix<double, 4, 1> col{ (control_core::cartPoleDynamics(xp, u0, p)
                                        - control_core::cartPoleDynamics(xm, u0, p)) / (2*eps) };
        An.col(j) = col;
    }

    // B numerica: stessa formula, perturbando u invece di x
    // 4. B numerica: si perturba u, non x
    Eigen::Matrix<double, 1, 1> up{ u0 };
    Eigen::Matrix<double, 1, 1> um{ u0 };
    up(0) = up(0) + eps;
    um(0) = um(0) - eps;
    Eigen::Matrix<double, 4, 1> Bn{ (control_core::cartPoleDynamics(x0, up, p)
                                - control_core::cartPoleDynamics(x0, um, p)) / (2 * eps) };
    // confronto elemento per elemento con due cicli for (i, j), EXPECT_NEAR tol 1e-6
    // poi un ciclo su i per B
    // 5. confronto
    for (int i{ 0 }; i < 4; ++i)
    {
        for (int j{ 0 }; j < 4; ++j)
        {
            EXPECT_NEAR(A(i, j), An(i, j), 1e-6);
        }
        EXPECT_NEAR(B(i), Bn(i), 1e-6);
    }
}

TEST(CartPole, EnergyConservedWithRk4)
{
    control_core::CartPoleParams p{};
    
    // x = [0; 0; 0.1; 0], u = 0, h = 1e-3
    Eigen::Matrix<double, 4, 1> x{ Eigen::Matrix<double, 4, 1>::Zero() };
    x(2,0) = 0.1;
    Eigen::Matrix<double, 1, 1> u{ Eigen::Matrix<double, 1, 1>::Zero() };
    double h = 1e-3;
    // lambda f(x, u) che cattura p e chiama cartPoleDynamics(x, u, p)
    auto f{
    [p](const Eigen::Matrix<double, 4, 1>& x, const Eigen::Matrix<double, 1, 1>& u) -> Eigen::Matrix<double, 4, 1>
    {
        return control_core::cartPoleDynamics(x, u, p);
    }
   };
    // tipo di ritorno esplicito -> Eigen::Matrix<double, 4, 1>

    // E0 = energia iniziale
    double E0{cartPoleEnergy(x, p)};
    for(int k{0}; k<1000; ++k)
    {
	x = control_core::rk4Step<4, 1>(f, x, u, h);
    }
    // 1000 passi: x = rk4Step<4, 1>(f, x, u, h)
    // E1 = energia finale
    double E1{cartPoleEnergy(x, p)};

    EXPECT_NEAR(E1, E0, 1e-8);
}
