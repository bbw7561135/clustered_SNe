
#include <cmath>
#include "../structure.H"
#include "initial_conditions.H"
#include "chevalier_ICs.H"


int Chevalier_ICs::setICparams( struct domain * theDomain )
{

    t = theDomain->theParList.t_min;
    return 0;

}

void Chevalier_ICs::initial( double * prim , double r )
{

    double s = 2.0;
    double n = 7.0;

    double g = 1.0;
    double q = 1.0;

    double rho1 = std::pow(r/t/g,-n)*std::pow(t,-3.);
    double rho2 = q*std::pow(r,-s);

    double R0 = std::pow( std::pow(t,n-3.)*std::pow(g,n)/q , 1./(n-s) );
    double r1 = 0.065*R0;
    if( r<r1 ) rho1 = std::pow(r1/t/g,-n)*std::pow(t,-3.);

    double rho = rho1+rho2;
    double v   = (r/t)*rho1/(rho1+rho2);
    double Z   = rho1/(rho1+rho2);

    double Pmin = rho*1e-5;

    prim[RHO] = rho;
    prim[PPP] = Pmin;
    prim[VRR] = v;
    prim[ZZZ] = Z;

}
