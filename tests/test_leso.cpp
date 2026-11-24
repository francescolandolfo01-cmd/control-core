#include <gtest/gtest.h>
#include <control_core/leso.h>
#include <control_core/state_space.h>
#include <cmath>

// Riferimenti da tools/ref_leso.py (omega0 = 50, b0 = 2, Ts = 0.001)

TEST(Leso, DiscretizationMatchesStateSpace)
{
    // leso(50.0, 2.0, 0.001)
    control_core::Leso leso(50, 2, 0.001);
    // A (3x3) e B (3x1) del modello esteso, con <<
    Eigen::Matrix<double, 3, 3> A = Eigen::Matrix<double, 3, 3>::Zero();
    A(0,1) = 1.0;
    A(1,2) = 1.0;
    Eigen::Matrix<double, 3, 1> B = Eigen::Matrix<double, 3, 1>::Zero();
    B(1) = 1.0;
    // StateSpace<3, 1> ss(A, B, 0.001, Eigen::Matrix<double, 3, 1>::Zero())
    control_core::StateSpace<3, 1> ss(A, B, 0.001, Eigen::Matrix<double, 3, 1>::Zero());
    // doppio for su i, j: EXPECT_NEAR(leso.Phi()(i, j), ss.Ad()(i, j), 1e-12)
    for(int i{0}; i<3; ++i) {
    	for(int j{0}; j<3; ++j) {
    		EXPECT_NEAR(leso.Phi()(i, j), ss.Ad()(i, j), 1e-12);
    	}
        EXPECT_NEAR(leso.Gamma()(i), ss.Bd()(i), 1e-12);
   }
    // for su i: EXPECT_NEAR(leso.Gamma()(i), ss.Bd()(i), 1e-12)
}

TEST(Leso, GainMatchesAckermann)
{
    // leso(50.0, 2.0, 0.001)
    control_core::Leso leso(50, 2, 0.001);
    EXPECT_NEAR(leso.gain()(0), 0.14631172649785795, 1e-10);
    EXPECT_NEAR(leso.gain()(1), 7.077705013255221, 1e-10);
    EXPECT_NEAR(leso.gain()(2), 116.00418067888494, 1e-10);
    // 3 EXPECT_NEAR con i valori di L stampati da ref_leso.py (17 cifre), tol 1e-10
}

TEST(Leso, ErrorMatrixHasTriplePole)
{
    // leso(50.0, 2.0, 0.001)
    control_core::Leso leso(50, 2, 0.001);
    // beta = e^(−50·0.001)   (variabile locale: tipo + nome)
    double beta{std::exp(-50*0.001)};
    Eigen::Matrix<double, 3, 3> N = Eigen::Matrix<double, 3, 3>::Zero();
    N = leso.errorMatrix() - beta*Eigen::Matrix<double, 3, 3>::Identity();
    // N (3x3) = leso.errorMatrix() − beta·I,  I = Eigen::Matrix<double, 3, 3>::Identity()
    EXPECT_LT((N*N*N).cwiseAbs().maxCoeff(), 1e-12);
    // EXPECT_LT((N*N*N).cwiseAbs().maxCoeff(), 1e-12)
}
