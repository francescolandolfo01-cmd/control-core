#include <gtest/gtest.h>
#include <control_core/leso.h>
#include <control_core/state_space.h>
#include <cmath>
#include <optional>

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

// Pianta + LESO (omega0 = 50, b0 = 2, Ts = 0.001) con d e u costanti.
// Restituisce il primo k (1..maxSteps) con |ẑ(2) − d| < tol, altrimenti nessun valore.
std::optional<int> firstStepWithin(double d, double u, double tol, int maxSteps)
{
    // leso(50.0, 2.0, 0.001)
    control_core::Leso leso(50, 2.0, 0.001);
    // pianta: StateSpace<3, 1> con A esteso (come nel primo test), B = [0; 2; 0], x0 = [0; 0; d]
    Eigen::Matrix<double, 3, 3> A = Eigen::Matrix<double, 3, 3>::Zero();
    A(0,1) = 1.0;
    A(1,2) = 1.0;
    Eigen::Matrix<double, 3, 1> B = Eigen::Matrix<double, 3, 1>::Zero();
    B(1) = 2.0;
    Eigen::Matrix<double, 3, 1> x0 = Eigen::Matrix<double, 3, 1>::Zero();
    x0(2) = d;

    control_core::StateSpace<3, 1> plant(A, B, 0.001, x0);
    // uv (Eigen::Matrix<double, 1, 1>) con uv << u;
    Eigen::Matrix<double, 1, 1> uv = Eigen::Matrix<double, 1, 1>::Zero();
    uv << u;
    for (int k{ 1 }; k <= maxSteps; ++k)
    {
        // 1. y = x della pianta    2. leso.step(u, y)    3. plant.step(uv)
	double y{plant.state()(0)};
	leso.step(u, y);
	plant.step(uv);
	
        // 4. se |leso.state()(2) − d| < tol → return k;
	if(std::abs(leso.state()(2) - d) < tol) {
		return k;
           }
    }
    return std::nullopt;
}

TEST(Leso, EstimatesConstantDisturbance)
{
    control_core::Leso leso(50, 2.0, 0.001);
    double d{-3.0};
    // stessa pianta con d = −3, u = 0.5, 1000 passi nello stesso ordine
    Eigen::Matrix<double, 3, 3> A = Eigen::Matrix<double, 3, 3>::Zero();
    A(0,1) = 1.0;
    A(1,2) = 1.0;
    Eigen::Matrix<double, 3, 1> B = Eigen::Matrix<double, 3, 1>::Zero();
    B(1) = 2.0;
    Eigen::Matrix<double, 3, 1> x0 = Eigen::Matrix<double, 3, 1>::Zero();
    x0(2) = d;
    control_core::StateSpace<3, 1> plant(A, B, 0.001, x0);
    // uv (Eigen::Matrix<double, 1, 1>) con uv << u;
    Eigen::Matrix<double, 1, 1> uv = Eigen::Matrix<double, 1, 1>::Zero();
    uv << 0.5;
    double u{0.5};
    for(int j{0}; j<1000; ++j) {
		        double y{plant.state()(0)};
                        leso.step(u, y);
        		plant.step(uv);
    }

  
    for(int i{0}; i<3; ++i) {
		EXPECT_NEAR(leso.state()(i), plant.state()(i), 1e-9);
    }
    // per i da 0 a 2: EXPECT_NEAR(leso.state()(i), plant.state()(i), 1e-9)
}

TEST(Leso, FirstStepWithinTolerance)
{
    // r1 = firstStepWithin(-3.0, 0.5, 1e-3, 1000): ASSERT_TRUE(r1.has_value()), poi EXPECT_EQ(*r1, 252)
    std::optional<int> r1 { firstStepWithin(-3.0, 0.5, 1e-3, 1000) };
    ASSERT_TRUE(r1.has_value());
    EXPECT_EQ(*r1, 252);
    // r2 = firstStepWithin(-3.0, 0.5, 1e-3, 200): EXPECT_FALSE(r2.has_value())
    std::optional<int> r2 { firstStepWithin(-3.0, 0.5, 1e-3, 200) };
    EXPECT_FALSE(r2.has_value());
}
