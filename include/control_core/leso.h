#ifndef CONTROL_CORE_LESO_H
#define CONTROL_CORE_LESO_H

#include <Eigen/Dense>
#include <cmath>

namespace control_core
{
// LESO discreto per ẍ = d + b0·u, stato esteso z = [x, ẋ, d]
class Leso
{
public:
    Leso(double omega0, double b0, double Ts)
      : m_Phi{ Eigen::Matrix<double, 3, 3>::Identity() }
      , m_Gamma{ Eigen::Matrix<double, 3, 1>::Zero() }
      , m_L{ Eigen::Matrix<double, 3, 1>::Zero() }
      , m_z{ Eigen::Matrix<double, 3, 1>::Zero() }
      , m_b0{ b0 }// m_L e m_z a ::Zero(), m_b0{ b0 }
    {
        m_Phi(0,1) = Ts;// Φ: tre elementi fuori diagonale (Ts, Ts²/2, Ts)
        m_Phi(1,2) = Ts;
        m_Phi(0,2) = 0.5*Ts*Ts;
	m_Gamma << 0.5*Ts*Ts, 
		   Ts,
   		   0.0;
        // Γ = [Ts²/2, Ts, 0]ᵀ
        // beta = e^(−omega0·Ts)   (variabile locale double, NON un membro)
        double beta{};
        beta = std::exp(-omega0*Ts);
        // L = [3(1−β), (1−β)²(5+β)/(2Ts), (1−β)³/Ts²]ᵀ
	m_L << 3*(1-beta),
	       std::pow((1-beta), 2)*(5 + beta)/(2*Ts),
               std::pow((1-beta), 3)/(Ts*Ts);
    }

    void step(double u, double y)
    {
        m_z = m_Phi*m_z + m_Gamma*m_b0*u + m_L*(y - m_z(0));// z = Φ z + Γ b0 u + L (y − z(0))
    }

    // state(), Phi(), Gamma(), gain(): const, restituiscono i membri
    Eigen::Matrix<double, 3, 1> const state() {return m_z;}
    Eigen::Matrix<double, 3, 3> const Phi() {return m_Phi;}
    Eigen::Matrix<double, 3, 1> const Gamma() {return m_Gamma;}
    Eigen::Matrix<double, 3, 1> const gain() {return m_L;}
    Eigen::Matrix<double, 3, 3> errorMatrix() {
		Eigen::Matrix<double, 1, 3> C = Eigen::Matrix<double, 1, 3>::Zero();
		C << 1.0, 0.0, 0.0;
		return m_Phi - m_L*C;
    }
    // errorMatrix() const: Φ − L C, con C = Eigen::Matrix<double, 1, 3> {1, 0, 0}

private:
	Eigen::Matrix<double, 3, 3> m_Phi;
        Eigen::Matrix<double, 3, 1> m_Gamma;
        Eigen::Matrix<double, 3, 1> m_L;
        Eigen::Matrix<double, 3, 1> m_z;
        double m_b0;
    // 5 membri: m_Phi (3x3), m_Gamma (3x1), m_L (3x1), m_z (3x1), m_b0 (double)
};
}

#endif
