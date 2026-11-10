#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "control_core/state_space.h"

TEST(StateSpace, DoubleIntegratorZoh)
{
    Eigen::Matrix<double, 2, 2> A;
    A << 0.0, 1.0,
         0.0, 0.0;
    Eigen::Matrix<double, 2, 1> B;
    B << 0.0, 1.0;
    const double Ts{ 0.1 };
    control_core::StateSpace<2, 1> sys{ A, B, Ts, Eigen::Matrix<double, 2, 1>::Zero() };

    // Ad = [1 Ts; 0 1]
    EXPECT_NEAR(sys.Ad()(0, 0), 1, 1e-12);
    EXPECT_NEAR(sys.Ad()(0, 1), Ts, 1e-12);
    EXPECT_NEAR(sys.Ad()(1, 0), 0, 1e-12);
    EXPECT_NEAR(sys.Ad()(1, 1), 1, 1e-12);
    // Bd = [Ts^2/2; Ts]
    EXPECT_NEAR(sys.Bd()(0, 0), Ts*Ts/2, 1e-12);
    EXPECT_NEAR(sys.Bd()(1, 0), Ts, 1e-12);
}

TEST(StateSpace, DoubleIntegratorStep)
{
    // stessi A, B, Ts e sys del test sopra
    Eigen::Matrix<double, 2, 2> A;
    A << 0.0, 1.0,
         0.0, 0.0;
    Eigen::Matrix<double, 2, 1> B;
    B << 0.0, 1.0;
    const double Ts{ 0.1 };
    control_core::StateSpace<2, 1> sys{ A, B, Ts, Eigen::Matrix<double, 2, 1>::Zero()};

    Eigen::Matrix<double, 1, 1> u;
    u << 1.0;
    for (int k = 0; k < 50; ++k)
    {
        sys.step(u);
    }
    // t = 50 * Ts = 5: posizione t^2/2, velocità t
    const double t{ 50 * Ts };
    EXPECT_NEAR(sys.state()(0), t*t/2, 1e-10);
    EXPECT_NEAR(sys.state()(1), t, 1e-10);
}
