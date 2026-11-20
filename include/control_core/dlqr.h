#ifndef CONTROL_CORE_DLQR_H
#define CONTROL_CORE_DLQR_H

#include <Eigen/Dense>

namespace control_core
{
// Iterazione di Riccati. Restituisce true se converge entro maxIter.
template <int Nx, int Nu>
bool dlqr(const Eigen::Matrix<double, Nx, Nx>& Ad,
          const Eigen::Matrix<double, Nx, Nu>& Bd,
          const Eigen::Matrix<double, Nx, Nx>& Q,
          const Eigen::Matrix<double, Nu, Nu>& R,
          int maxIter, double tol,
          Eigen::Matrix<double, Nx, Nx>& P,
          Eigen::Matrix<double, Nu, Nx>& K)
{
    P = Q;
    for (int j{0}; j<maxIter; ++j)
    {
        // S  (Nu x Nu) = R + Bdᵀ P Bd
	Eigen::Matrix<double, Nu, Nu> S = R + Bd.transpose()*P*Bd;
        // G  (Nu x Nx) = S.ldlt().solve( Bdᵀ P Ad )
	Eigen::Matrix<double, Nu, Nx> G = S.ldlt().solve( Bd.transpose()*P*Ad );
        // Pn (Nx x Nx) = Q + Adᵀ P Ad − Adᵀ P Bd G
	Eigen::Matrix<double, Nx, Nx> Pn = Q + Ad.transpose()*P*Ad - Ad.transpose()*P*Bd*G;
        // diff = massimo di |Pn − P| elemento per elemento
	double diff = ((Pn - P).cwiseAbs()).maxCoeff();
        // P = Pn; se diff < tol → calcola K con il P nuovo e return true
	P = Pn;
        if(diff < tol) {
		 S = R + Bd.transpose()*P*Bd;
		 K = S.ldlt().solve( Bd.transpose()*P*Ad );
		 return true;
        }
    }
    return false;
}
}

#endif
