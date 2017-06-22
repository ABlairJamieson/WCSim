
#include "GraphTool.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

//=========================================================================
GraphTool::GraphTool( std::string afname, std::string axname, std::string ayname,
		      GraphToolFunctor* xfcn, GraphToolFunctor* yfcn ) :
  fname(afname), xname(axname), yname(ayname), fcnx(xfcn), fcny(yfcn) {
  parse_file();
}

//=========================================================================
void GraphTool::parse_file(){
  std::cout<<"GraphTool::parse_file"<<std::endl;
  if ( fname.size() == 0 ) {
    std::cout<<"GraphTool::parse_file (Warning) no file provided"<<std::endl;
    ftg = nullptr;
    return;
  }
  std::ifstream fin( fname.c_str() );
  if (!fin) std::cout<<"GraphTool::parse_file (Warning) "
	      " could not open file: "<<fname<<std::endl;
  double x,y;
  std::vector<double> xx;
  std::vector<double> yy;
  std::string s;
  // ignore comment lines
  while ( fin && fin.peek() == '#' ) getline( fin, s );
  while ( fin && fin >> x >> y ){
    if (fcnx != nullptr) x = fcnx->operator()( x );
    if (fcny != nullptr) y = fcny->operator()( y );
    xx.push_back( x );
    yy.push_back( y );
  }
  std::ostringstream ss;
  ss << "GraphTool::parse_file( "<<fname<<" ) stopped after x="<<x<<" y="<<y;
  //print_stream_error( fin, ss.str());
  if ( xx.size() > 0 ){
    ftg = new TGraph( xx.size(), &xx[0], &yy[0] );
    // build a unique title
    std::ostringstream os;
    os << fname <<" ;"<< xname << " ;"<<yname;
    ftg->SetTitle( os.str().c_str() );
  } else {
    ftg = nullptr;
  }
}


//=========================================================================
// print table from GraphTool's TGraph
ostream & operator<<( ostream &os, const GraphTool & d ){
  TGraph * g = d.get_TGraph();
  if ( g ){
    os << "Printing table for " << g->GetTitle() << std::endl;
    double const * x = g->GetX();
    double const * y = g->GetY();
    for ( int i=0; i<g->GetN(); ++i){
    
      os << std::scientific << std::setprecision(8) << x[i]
	 << " " << std::scientific << std::setprecision(8) << y[i]
	 << std::endl;
      os << std::setprecision(6);
    }
  }
  return os;
}
