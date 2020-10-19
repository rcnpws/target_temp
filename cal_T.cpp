////////////////////////////////////////////////
//
//  This program gives the temperature distribution inside a target
//  with energy loss of beam by solving the Heat equation.
//   
//  Useage: 
//  $> root -l cal_T.cpp
//
//  Set parameters in #define...
//
//  ver. 1.0 (2020/10/19) T. Furuno
//
////////////////////////////////////////////////


#include <iostream>
#include <cmath>
#include <TStyle.h>
using namespace std;

#define rmax 3.0	   // radius of target (cm)
#define bT 300.0           // temperature at the boundary (K)
#define dE_beam 1200.0     // energy loss throught the target (MeV)
#define thick_target 4.46  // thickness/range of the target (g/cm2)
#define rho 7.8            // density of the target (g/cm3)
#define l   4.02           // thermal conductivity of the target (W/cm K)
#define r0  0.1            // radius of the beam (cm)
#define beam_cur 100.0     //  beam current (pnA)

#define max_T 1000         // maximum tepmerature search (K) (option)

double f2(double r,double T,double u);	// 2nd order differential of T

//＝＝＝＝＝＝＝＝＝＝
// main
//＝＝＝＝＝＝＝＝＝＝
cal_T(){
  double T;		   // temperature (K)
  double dT = 1;	   // temperature step (K)
  double u;		   // first-order differential of T
  double fT;		   // temperature at center
  double aT;	           // solution of the center temerature
  double tmp_bT = 10000;   // temporary solution
  double r;		   // radius (cm)
  double dr = 0.001;	   // step of r (cm)
  
  /*** 1. find the center temperature which satisfies the boundary condition ***/
  for(fT=0.0; fT<max_T; fT+=dT ) {
    T = fT;
    u = 0;
    for(r=0.01; r<rmax; r+=dr) {
      T += dr*u;
      u += dr*f2(r,T,u);
    }
    if( fabs(T-bT) < fabs(tmp_bT-bT) ) {	 
      aT = fT;
      tmp_bT = T;
    }
  }
  
  printf("center temperature: %.0f K\n", aT);

  /*** 2. calculate the temperature distribution with the solution found in 1. ***/
  T = aT;
  u = 0;
  float d[2000];
  std::vector<float> r_plot;
  std::vector<float> T_plot;
  int gr_cnt=0;
  for(r=0.01; r<rmax; r+=dr) {
    T += dr*u;
    u += dr*f2(r,T,u);
    d[(int)(r*100)] = T;
    r_plot.push_back(r);
    T_plot.push_back(T);
    gr_cnt++;
  }
  
  /* 3. Draw graph */
  TGraph *graph = new TGraph(gr_cnt, &r_plot[0], &T_plot[0]);
  graph->SetTitle("title");
  graph->GetXaxis()->SetTitle("r (cm)");
  graph->GetYaxis()->SetTitle("T (K)");
  graph->SetLineWidth(2);
  graph -> Draw("AL");
}

//＝＝＝＝＝＝＝＝＝＝
// 2nd order differential of T
//＝＝＝＝＝＝＝＝＝＝
double f2(double r,double T,double u) {
  double uu;		// 2nd order differential of T
  double W;		// heat with unit volume (J/cm3/s)
  
  //*** calc. uu with the thermal conduction equation ***/
  W = dE_beam/thick_target*beam_cur*1.0e-3/(3.14*r0*r0)*rho;
  if(r<=r0) uu = -W/l-u/r;	// with heat
  else uu = -u/r;		// without heat
  
  return uu;
}
