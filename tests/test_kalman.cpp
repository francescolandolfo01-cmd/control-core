#include <gtest/gtest.h>
#include <control_core/kalman.h>
#include <cmath>
#include <control_core/cart_pole.h>
#include <control_core/state_space.h>

// Caso scalare: Ad = 1, Bd = 0, C = 1, Qn = 1, Rn = 1, x0 = 0, P0 = 1
// Valori attesi calcolati a mano (vedi NOTES 22/11):
//   un passo con y = 3: x = 2, P = L = 2/3
//   regime: P = L = 1/φ = (√5 − 1)/2

void setupCartPoleKf(Eigen::Matrix<double, 4, 4>& Ad,
                     Eigen::Matrix<double, 4, 1>& Bd,
                     Eigen::Matrix<double, 2, 4>& C,
                     Eigen::Matrix<double, 4, 4>& Qn,
                     Eigen::Matrix<double, 2, 2>& Rn)
{
    // cartPoleLinearUpright → StateSpace<4, 1>(A, B, 0.02, …) → Ad, Bd   (come setupCartPole)
    control_core::CartPoleParams p{};
    Eigen::Matrix<double, 4, 4> A;
    Eigen::Matrix<double, 4, 1> B;
    control_core::cartPoleLinearUpright(p, A, B);
    control_core::StateSpace<4, 1> ss(A, B, 0.02, Eigen::Matrix<double, 4, 1>::Zero());    // cartPoleLinearUpright → S>
    Ad = ss.Ad();
    Bd = ss.Bd();
    C = Eigen::Matrix<double, 2, 4>::Zero();
    C << 1.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0;
    
    Qn = 1e-4* Eigen::Matrix<double, 4, 4>::Identity();
    Rn = 1e-3* Eigen::Matrix<double, 2, 2>::Identity();
    // C con <<: righe [1 0 0 0] e [0 0 1 0]
    // Qn = 1e-4 · Identity, Rn = 1e-3 · Identity   ← si ASSEGNANO, sono parametri &
}

TEST(Kalman, OneStepByHand)
{
    // 7 Matrix<double, 1, 1>: Ad, Bd, C, Qn, Rn, x0, P0, ciascuna assegnata con <<
    Eigen::Matrix<double, 1, 1> Ad = Eigen::Matrix<double, 1, 1>::Zero();
    Ad << 1.0;
    Eigen::Matrix<double, 1, 1> Bd = Eigen::Matrix<double, 1, 1>::Zero();
    Bd << 0.0;
    Eigen::Matrix<double, 1, 1> C = Eigen::Matrix<double, 1, 1>::Zero();
    C << 1.0;
    Eigen::Matrix<double, 1, 1> Qn = Eigen::Matrix<double, 1, 1>::Zero();
    Qn << 1.0;
    Eigen::Matrix<double, 1, 1> Rn = Eigen::Matrix<double, 1, 1>::Zero();
    Rn << 1.0;
    Eigen::Matrix<double, 1, 1> x0 = Eigen::Matrix<double, 1, 1>::Zero();
    x0 << 0.0;
    Eigen::Matrix<double, 1, 1> P0 = Eigen::Matrix<double, 1, 1>::Zero();
    P0 << 1.0;

    // KalmanFilter<1, 1, 1> kf(Ad, Bd, C, Qn, Rn, x0, P0);
    control_core::KalmanFilter<1, 1, 1> kf(Ad, Bd, C, Qn, Rn, x0, P0);
    // u e y: Matrix<double, 1, 1>, u << 0.0; y << 3.0;
    Eigen::Matrix<double, 1, 1> u = Eigen::Matrix<double, 1, 1>::Zero();
    u << 0.0;
    Eigen::Matrix<double, 1, 1> y = Eigen::Matrix<double, 1, 1>::Zero();
    y << 3.0;

    kf.predict(u);
    kf.update(y);    
    // kf.predict(u); kf.update(y);
    EXPECT_NEAR(kf.state()(0), 2.0, 1e-15);
    EXPECT_NEAR(kf.covariance()(0, 0), 2.0/3.0, 1e-15);
    EXPECT_NEAR( kf.gain()(0, 0), 2.0/3.0, 1e-15);
    // 3 EXPECT_NEAR, tol 1e-15: kf.state()(0), kf.covariance()(0, 0), kf.gain()(0, 0)
}

TEST(Kalman, ScalarConvergesToGoldenRatio)
{
    // stesse 7 Matrix<double, 1, 1> di OneStepByHand (ogni TEST è separato: vanno riscritte)
    // KalmanFilter<1, 1, 1> kf(…);
    // u << 0.0; y << 0.0;
    Eigen::Matrix<double, 1, 1> Ad = Eigen::Matrix<double, 1, 1>::Zero();
    Ad << 1.0;
    Eigen::Matrix<double, 1, 1> Bd = Eigen::Matrix<double, 1, 1>::Zero();
    Bd << 0.0;
    Eigen::Matrix<double, 1, 1> C = Eigen::Matrix<double, 1, 1>::Zero();
    C << 1.0;
    Eigen::Matrix<double, 1, 1> Qn = Eigen::Matrix<double, 1, 1>::Zero();
    Qn << 1.0;
    Eigen::Matrix<double, 1, 1> Rn = Eigen::Matrix<double, 1, 1>::Zero();
    Rn << 1.0;
    Eigen::Matrix<double, 1, 1> x0 = Eigen::Matrix<double, 1, 1>::Zero();
    x0 << 0.0;
    Eigen::Matrix<double, 1, 1> P0 = Eigen::Matrix<double, 1, 1>::Zero();
    P0 << 1.0;

    // KalmanFilter<1, 1, 1> kf(Ad, Bd, C, Qn, Rn, x0, P0);
    control_core::KalmanFilter<1, 1, 1> kf(Ad, Bd, C, Qn, Rn, x0, P0);
    // u e y: Matrix<double, 1, 1>, u << 0.0; y << 3.0;
    Eigen::Matrix<double, 1, 1> u = Eigen::Matrix<double, 1, 1>::Zero();
    u << 0.0;

    Eigen::Matrix<double, 1, 1> y = Eigen::Matrix<double, 1, 1>::Zero();
    y << 0.0;

    for(int i{0}; i<100; ++i)
    {
	 kf.predict(u);
         kf.update(y);
    }
    // for 100 volte: kf.predict(u); kf.update(y);
    const double invPhi{ (std::sqrt(5.0) - 1.0) / 2.0 };  // ← serve #include <cmath>
    // 2 EXPECT_NEAR, tol 1e-12: kf.covariance()(0, 0) e kf.gain()(0, 0) contro invPhi
    EXPECT_NEAR(kf.covariance()(0, 0), invPhi, 1e-12);
    EXPECT_NEAR( kf.gain()(0, 0), invPhi, 1e-12);

}

TEST(Kalman, CartPoleMatchesScipyDualDare)
{
    Eigen::Matrix<double, 4, 4> Ad;
    Eigen::Matrix<double, 4, 1> Bd;
    Eigen::Matrix<double, 2, 4> C;
    Eigen::Matrix<double, 4, 4> Qn;
    Eigen::Matrix<double, 2, 2> Rn;

    // setupCartPoleKf(…); x0 = Zero, P0 = Identity
    setupCartPoleKf(Ad, Bd, C, Qn, Rn);
    // control_core::KalmanFilter<4, 1, 2> kf(…);
    Eigen::Matrix<double, 4, 1> x0 = Eigen::Matrix<double, 4, 1>::Zero();
    Eigen::Matrix<double, 4, 4> P0 = Eigen::Matrix<double, 4, 4>::Identity();
    control_core::KalmanFilter<4, 1, 2> kf(Ad, Bd, C, Qn, Rn, x0, P0);
    // u (1x1) = 0, y (2x1) = 0
    Eigen::Matrix<double, 2, 1> y = Eigen::Matrix<double, 2, 1>::Zero();
    Eigen::Matrix<double, 1, 1> u = Eigen::Matrix<double, 1, 1>::Zero();
    // 1000 volte: predict(u); update(y);
    for(int i{0}; i<1000; ++i)
    {
         kf.predict(u);
         kf.update(y);
    }
    EXPECT_NEAR( kf.gain()(0, 0), 0.2842066472805984, 1e-10);
    EXPECT_NEAR( kf.gain()(1, 0), 0.2679502526693914, 1e-10);
    EXPECT_NEAR( kf.gain()(2, 0), -0.00081252814282948, 1e-10);
    EXPECT_NEAR( kf.gain()(3, 0), -0.00982369817027965, 1e-10);
    EXPECT_NEAR( kf.gain()(0, 1), -0.00081252814282948, 1e-10);
    EXPECT_NEAR( kf.gain()(1, 1), -0.02384666499618483, 1e-10);
    EXPECT_NEAR( kf.gain()(2, 1), 0.31036183791136324, 1e-10);
    EXPECT_NEAR( kf.gain()(3, 1), 0.8120211505500662, 1e-10);
    // 8 EXPECT_NEAR su kf.gain()(i, j), tol 1e-10            ← da ref_kalman.py
    // 4 EXPECT_NEAR su kf.covariance()(i, i), tol 1e-12
    EXPECT_NEAR(kf.covariance()(0, 0), 0.0002842066472806, 1e-12);
    EXPECT_NEAR(kf.covariance()(1, 1), 0.00531976903395645, 1e-12);
    EXPECT_NEAR(kf.covariance()(2, 2), 0.00031036183791136, 1e-12);
    EXPECT_NEAR(kf.covariance()(3, 3), 0.0107708530963175, 1e-12);
}
