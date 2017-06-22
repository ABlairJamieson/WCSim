
#include "LEDTimeDistro.hh"
#include "TRandom.h"
#include "TAxis.h"

//#include <iostream>

//=========================================================================
LEDTimeDistro::LEDTimeDistro( double atmin, double atmax ) :
  GraphTool("","time (ns)", "intensity", nullptr, nullptr ),
  ftmin(atmin), ftmax(atmax ){}

//=========================================================================
LEDTimeDistro::LEDTimeDistro( std::string afname ) :
  GraphTool( afname, "time (ns)", "intensity", nullptr, nullptr ),
  ftmin(-9999.), ftmax(-9999.) {
}
  
//=========================================================================
double LEDTimeDistro::random_time() const{
  if ( ftmin == ftmax ) {// select reject based on TGraph
    double itmax = ftg->GetMaximum();
    double trand;
    double tthrow;
    double tmin = ftg->GetXaxis()->GetXmin();
    double tmax = ftg->GetXaxis()->GetXmax();
    do {
      trand = tmin + (tmax-tmin) * gRandom->Rndm();
      tthrow = itmax * gRandom->Rndm();
    } while ( tthrow > ftg->Eval( trand ) );
    // std::cout<<"LEDTimeDistro::random_time = "<<trand<<std::endl;
    return trand;
  } else { // uniform random
    return ftmin + (ftmax-ftmin) * gRandom->Rndm();
  }
}
