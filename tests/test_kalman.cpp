#include <gtest/gtest.h>
#include <control_core/kalman.h>
#include <cmath>

// Caso scalare: Ad = 1, Bd = 0, C = 1, Qn = 1, Rn = 1, x0 = 0, P0 = 1
// Valori attesi calcolati a mano (vedi NOTES 22/11):
//   un passo con y = 3: x = 2, P = L = 2/3
//   regime: P = L = 1/φ = (√5 − 1)/2

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
