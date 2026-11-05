#ifndef CONTROL_CORE_FIRST_ORDER_ZOH_H
#define CONTROL_CORE_FIRST_ORDER_ZOH_H

#include <cmath>

namespace control_core
{
class FirstOrderZoh
{
public:
    FirstOrderZoh(double K, double tau, double Ts, double x0)
        : m_a{ std::exp(-Ts/tau) }
        , m_b{ K*(1-m_a) }
        , m_x{ x0 }
    {
    }

    void step(double u) { m_x = m_a*m_x + m_b*u; }
    double state() const { return m_x; }
    double a() const { return m_a; }
    double b() const { return m_b; }

private:
    double m_a{};
    double m_b{};
    double m_x{};
};
}

#endif
