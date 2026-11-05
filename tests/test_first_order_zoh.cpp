#include <gtest/gtest.h>
#include <cmath>
#include "control_core/first_order_zoh.h"

TEST(FirstOrderZoh, Coefficients)
{
    control_core::FirstOrderZoh sys{2.0, 0.5, 0.1, 0.0};   // K, tau, Ts, x0
    EXPECT_NEAR(sys.a(), std::exp(-0.2), 1e-12);
    EXPECT_NEAR(sys.b(), 2.0*(1- std::exp(-0.2)), 1e-12);
}

TEST(FirstOrderZoh, StepMatchesAnalytic)
{
    control_core::FirstOrderZoh sys{2.0, 0.5, 0.1, 0.0};
    for (int k = 0; k < 50; ++k)
    {
        sys.step(1.0);// ??? gradino u = 1
    }
    // dopo 50 passi t = 50 * Ts = 5: x(5) = K * (1 - e^(-5/tau))
    EXPECT_NEAR(sys.state(), 2.0*(1-std::exp(-5/0.5)), 1e-10);
}
