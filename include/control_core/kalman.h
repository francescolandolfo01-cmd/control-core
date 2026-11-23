#ifndef CONTROL_CORE_KALMAN_H
#define CONTROL_CORE_KALMAN_H

#include <Eigen/Dense>

namespace control_core
{
template <int Nx, int Nu, int Ny>
class KalmanFilter
{
public:
    KalmanFilter(const Eigen::Matrix<double, Nx, Nx>& Ad,
                 const Eigen::Matrix<double, Nx, Nu>& Bd,
                 const Eigen::Matrix<double, Ny, Nx>& C,
                 const Eigen::Matrix<double, Nx, Nx>& Qn,
                 const Eigen::Matrix<double, Ny, Ny>& Rn,
                 const Eigen::Matrix<double, Nx, 1>& x0,
                 const Eigen::Matrix<double, Nx, Nx>& P0)
      :m_Ad{ Ad }  // lista di inizializzazione: m_Ad{ Ad }, … per tutti i membri; m_L parte a ::Zero()
      ,m_Bd{ Bd }
      ,m_C{ C }
      ,m_Qn{ Qn }
      ,m_Rn{ Rn }
      ,m_x{ x0 }  // lista di inizializzazione: m_Ad{ Ad }, … per tutti i membri; m_L parte a ::Zero()
      ,m_P{ P0 }
      ,m_L{ Eigen::Matrix<double, Nx, Ny>::Zero() } 
    {
    }

    void predict(const Eigen::Matrix<double, Nu, 1>& u)
    {
        // x = Ad x + Bd u
	m_x = m_Ad*m_x + m_Bd*u;
	m_P = m_Ad*m_P*m_Ad.transpose() + m_Qn;
        // P = Ad P Adᵀ + Qn
    }

    void update(const Eigen::Matrix<double, Ny, 1>& y)
    {
        // S (Ny x Ny) = C P Cᵀ + Rn
	 Eigen::Matrix<double, Ny, Ny> S = m_C*m_P*m_C.transpose() + m_Rn;
        // m_L (Nx x Ny) = ( S.ldlt().solve(C P) )ᵀ   ← si ASSEGNA al membro, non si ridichiara
	m_L = (S.ldlt().solve(m_C*m_P)).transpose();
        // x = x + L (y − C x)
	m_x = m_x + m_L*(y - m_C*m_x);
        // P = (I − L C) P,  con I = Eigen::Matrix<double, Nx, Nx>::Identity()
	//m_P = ( Eigen::Matrix<double, Nx, Nx>::Identity() - m_L*m_C)*m_P;
	Eigen::Matrix<double, Nx, Nx> IKC = Eigen::Matrix<double, Nx, Nx>::Identity() -m_L*m_C;
	m_P = IKC*m_P*IKC.transpose() + m_L*m_Rn*m_L.transpose();
    }

    Eigen::Matrix<double, Nx, 1> state() const { return m_x; }
    Eigen::Matrix<double, Nx, Nx> covariance() const { return m_P; }
    Eigen::Matrix<double, Nx, Ny> gain() const { return m_L; }

private:
    // 8 membri: m_Ad, m_Bd, m_C, m_Qn, m_Rn, m_x, m_P, m_L
    Eigen::Matrix<double, Nx, Nx> m_Ad;
    Eigen::Matrix<double, Nx, Nu> m_Bd;
    Eigen::Matrix<double, Ny, Nx> m_C;
    Eigen::Matrix<double, Nx, Nx> m_Qn;
    Eigen::Matrix<double, Ny, Ny> m_Rn;
    Eigen::Matrix<double, Nx, 1> m_x;
    Eigen::Matrix<double, Nx, Nx> m_P;
    Eigen::Matrix<double, Nx, Ny> m_L;
};
}

#endif
