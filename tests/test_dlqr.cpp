#include <gtest/gtest.h>
#include <control_core/cart_pole.h>
#include <control_core/state_space.h>
#include <control_core/dlqr.h>

// Riferimenti da tools/ref_dare.py
// (cart-pole in alto, Ts = 0.02, Q = diag(1, 1, 10, 1), R = 0.1)

// Prepara Ad, Bd, Q, R: lo stesso codice del main di prova
void setupCartPole(Eigen::Matrix<double, 4, 4>& Ad,
                   Eigen::Matrix<double, 4, 1>& Bd,
                   Eigen::Matrix<double, 4, 4>& Q,
                   Eigen::Matrix<double, 1, 1>& R)
{
    control_core::CartPoleParams p{};
    Eigen::Matrix<double, 4, 4> A;
    Eigen::Matrix<double, 4, 1> B;
    control_core::cartPoleLinearUpright(p, A, B);
    control_core::StateSpace<4, 1> ss(A, B, 0.02, Eigen::Matrix<double, 4, 1>::Zero());    // cartPoleLinearUpright → StateSpace<4, 1> → Ad = ss.Ad(); Bd = ss.Bd();
    Ad = ss.Ad();
    Bd = ss.Bd();
    Q = Eigen::Matrix<double, 4, 4>::Zero();
    Q(0,0) = 1.0;
    Q(1,1) = 1.0;
    Q(2,2) = 10.0;
    Q(3,3) = 1.0;
    // i 4 elementi diagonali di Q: 1, 1, 10, 1
    // R = 0.1 con <<
    R << 0.1;    // Q e R come nel main di prova (si ASSEGNANO: sono parametri &)
}

TEST(Dlqr, MatchesScipyDare)
{
    Eigen::Matrix<double, 4, 4> Ad;
    Eigen::Matrix<double, 4, 1> Bd;
    Eigen::Matrix<double, 4, 4> Q;
    Eigen::Matrix<double, 1, 1> R;
    setupCartPole(Ad, Bd, Q, R);

    Eigen::Matrix<double, 4, 4> P;
    Eigen::Matrix<double, 1, 4> K;

    bool ok = control_core::dlqr<4, 1>(Ad, Bd, Q, R, 100000, 1e-12, P, K);
    // setupCartPole, poi dlqr<4, 1>(…, 100000, 1e-12, P, K)
    ASSERT_TRUE(ok);
    EXPECT_NEAR(P(0,0), 90.97337676300165, 1e-8);
    EXPECT_NEAR(P(1,1), 86.56749476577055, 1e-8);
    EXPECT_NEAR(P(2,2), 1280.1640468383189, 1e-8);
    EXPECT_NEAR(P(3,3), 55.35540981720928, 1e-8);
    EXPECT_NEAR(K(0,0), -2.719429743878661, 1e-10);
    EXPECT_NEAR(K(0,1), -4.9479141334085375, 1e-10);
    EXPECT_NEAR(K(0,2), -41.60979870360231, 1e-10);
    EXPECT_NEAR(K(0,3), -9.375339783536937, 1e-10);
    // EXPECT_TRUE sul valore restituito
    // 4 EXPECT_NEAR su P(i, i), tol 1e-8   ← valori diag(P) da Python
    // 4 EXPECT_NEAR su K(0, j), tol 1e-10  ← valori K da Python
}

TEST(Dlqr, SatisfiesDare)
{
    Eigen::Matrix<double, 4, 4> Ad;
    Eigen::Matrix<double, 4, 1> Bd;
    Eigen::Matrix<double, 4, 4> Q;
    Eigen::Matrix<double, 1, 1> R;
    setupCartPole(Ad, Bd, Q, R);

    Eigen::Matrix<double, 4, 4> P;
    Eigen::Matrix<double, 1, 4> K;

    bool ok = control_core::dlqr<4, 1>(Ad, Bd, Q, R, 100000, 1e-12, P, K);
    ASSERT_TRUE(ok);
    Eigen::Matrix<double, 4, 4> res = Q + Ad.transpose()*P*Ad - Ad.transpose()*P*Bd*K - P;
    // setupCartPole + dlqr
    // residuo (4x4) = Q + Adᵀ P Ad − Adᵀ P Bd K − P
    EXPECT_LT((res.cwiseAbs()).maxCoeff(), 1e-8);
    // EXPECT_LT(massimo di |residuo|, 1e-8)
}

TEST(Dlqr, ClosedLoopConverges)
{
    Eigen::Matrix<double, 4, 4> Ad;
    Eigen::Matrix<double, 4, 1> Bd;
    Eigen::Matrix<double, 4, 4> Q;
    Eigen::Matrix<double, 1, 1> R;
    setupCartPole(Ad, Bd, Q, R);

    Eigen::Matrix<double, 4, 4> P;
    Eigen::Matrix<double, 1, 4> K;

    bool ok = control_core::dlqr<4, 1>(Ad, Bd, Q, R, 100000, 1e-12, P, K);
    ASSERT_TRUE(ok);
    // setupCartPole + dlqr
    // Acl (4x4) = Ad − Bd K
    Eigen::Matrix<double, 4, 4> Acl = Ad - Bd*K;
    Eigen::Matrix<double, 4, 1> x0 = Eigen::Matrix<double, 4, 1>::Zero();
    x0(2, 0) = 0.1;
    Eigen::Matrix<double, 4, 1> x{x0};
    for(int i{0}; i<1000; ++i){
    	x = Acl*x;
    }
    // x0 = [0; 0; 0.1; 0], 1000 passi x = Acl * x
    EXPECT_LT(x.norm(), 1e-9);
}
