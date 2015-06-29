enum{_HLL_,_HLLC_};

#include "paul.h"

int riemann_solver = 0;
int rt_flag = 0;

void setRiemannParams( struct domain * theDomain ){
   riemann_solver = theDomain->theParList.Riemann_Solver;
   rt_flag = theDomain->theParList.rt_flag;
}

void prim2cons( double * , double * , double );
void flux( double * , double * );
void getUstar( double * , double * , double , double );
void vel( double * , double * , double * , double * , double * , double );
double get_eta( double * , double * , double );

void riemann( struct cell * cL , struct cell * cR, double r , double dAdt ){

   double primL[NUM_Q];
   double primR[NUM_Q];

   double drL = .5*cL->dr;
   double drR = .5*cR->dr;

   int q;
   for( q=0 ; q<NUM_Q ; ++q ){
      primL[q] = cL->prim[q] + cL->grad[q]*drL;
      primR[q] = cR->prim[q] - cR->grad[q]*drR;
   }

   double Sl,Sr,Ss;

   vel( primL , primR , &Sl , &Sr , &Ss , r );

   double Fl[NUM_Q];
   double Fr[NUM_Q];
   double Ul[NUM_Q];
   double Ur[NUM_Q];

   double Flux[NUM_Q];

   double w = cL->wiph;

   if( w < Sl ){
      flux( primL , Fl );
      prim2cons( primL , Ul , 1.0 );

      for( q=0 ; q<NUM_Q ; ++q ){
         Flux[q] = Fl[q] - w*Ul[q];
      }
   }else if( w > Sr ){
      flux( primR , Fr );
      prim2cons( primR , Ur , 1.0 );

      for( q=0 ; q<NUM_Q ; ++q ){
         Flux[q] = Fr[q] - w*Ur[q];
      }
   }else{
      if( riemann_solver == _HLL_ ){
         double Fstar;
         double Ustar;
         double aL =  Sr;
         double aR = -Sl;
 
         prim2cons( primL , Ul , 1.0 );
         prim2cons( primR , Ur , 1.0 );
         flux( primL , Fl );
         flux( primR , Fr );

         for( q=0 ; q<NUM_Q ; ++q ){
            Fstar = ( aL*Fl[q] + aR*Fr[q] + aL*aR*( Ul[q] - Ur[q] ) )/( aL + aR );
            Ustar = ( aR*Ul[q] + aL*Ur[q] + Fl[q] - Fr[q] )/( aL + aR );

            Flux[q] = Fstar - w*Ustar;
         }
      }else{
         double Ustar[NUM_Q];
         double Uk[NUM_Q];
         double Fk[NUM_Q];
         if( w < Ss ){
            prim2cons( primL , Uk , 1.0 );
            getUstar( primL , Ustar , Sl , Ss ); 
            flux( primL , Fk ); 

            for( q=0 ; q<NUM_Q ; ++q ){
               Flux[q] = Fk[q] + Sl*( Ustar[q] - Uk[q] ) - w*Ustar[q];
            }    
         }else{
            prim2cons( primR , Uk , 1.0 );
            getUstar( primR , Ustar , Sr , Ss ); 
            flux( primR , Fk ); 

            for( q=0 ; q<NUM_Q ; ++q ){
               Flux[q] = Fk[q] + Sr*( Ustar[q] - Uk[q] ) - w*Ustar[q];
            } 
         } 
      }
   }

   if( rt_flag ){
      double prim[NUM_Q];
      double consL[NUM_Q];
      double consR[NUM_Q];
      prim2cons( cL->prim , consL , 1.0 );
      prim2cons( cR->prim , consR , 1.0 );
      double gprim[NUM_Q];
      double gcons[NUM_Q];
      for( q=0 ; q<NUM_Q ; ++q ){
         prim[q] = .5*(primL[q]+primR[q]);
         gprim[q] = (cR->prim[q] - cL->prim[q])/(drL+drR);
         gcons[q] = (consR[q] - consL[q])/(drL+drR);
      }
      double eta = get_eta( prim , gprim , r );
      for( q=0 ; q<NUM_Q ; ++q ){
         Flux[q] += -eta*gcons[q];
      }
   }

   for( q=0 ; q<NUM_Q ; ++q ){
      cL->cons[q] -= Flux[q]*dAdt;
      cR->cons[q] += Flux[q]*dAdt;
   }

}


