namespace control_core
{
struct CartPoleParams { double M{ 1.0 }; double m{ 0.1 }; double l{ 0.5 }; double g{ 9.81 }; };

inline Eigen::Matrix<double, 4, 1> cartPoleDynamics(const Eigen::Matrix<double, 4, 1>& x,
                                                    const Eigen::Matrix<double, 1, 1>& u,
                                                    const CartPoleParams& p)
{
    // 1. Estrai le componenti che servono in double con un nome leggibile
    const double pdot{ x(1) };
    const double theta{ x(2) };
    const double thetadot{ x(3) };
    const double F{ u(0) };

    // 2. Grandezze di supporto
    const double s{ std::sin(theta) };
    const double c{ std::cos(theta)  };
    const double D{ p.M + p.m*s*s };

    // 3. Le due accelerazioni: copia le formule del commento
    //    usando p.M, p.m, p.l, p.g e le variabili sopra
    const double pddot{ (F + p.m*p.l*thetadot*thetadot*s-p.m*p.g*s*c)/D };
    const double thetaddot{ (p.g*s - pddot*c)/p.l };

    // 4. Costruisci il vettore xdot e restituiscilo
    Eigen::Matrix<double, 4, 1> xdot{ Eigen::Matrix<double, 4, 1>::Zero() };
    xdot << pdot, pddot, thetadot, thetaddot;
    return xdot;
}

inline void cartPoleLinearUpright(const CartPoleParams& p,
                                  Eigen::Matrix<double, 4, 4>& A,
                                  Eigen::Matrix<double, 4, 1>& B)
{
    A = Eigen::Matrix<double, 4, 4>::Zero();
    A(0,1) = 1.0;
    A(1,2) = -p.m*p.g/p.M;
    A(2,3) = 1.0;
    A(3,2) = (p.M + p.m)*p.g/(p.M*p.l);
    // A tutta a zero con ::Zero(), poi i 4 elementi non nulli uno per uno: A(0,1), A(1,2), A(2,3), A(3,2)
    // B con << e 4 valori separati da virgole, usando p.M, p.m, p.l, p.g
    B << 0.0, 1/p.M, 0.0, -1/(p.M*p.l);
}

}
