#ifndef GraphTool_h
#define GraphTool_h 1

#include <string>
#include "TGraph.h"

/// GraphTool.hh
/// Contains tools for making a TGraph from a file, and have arbitrary
/// scaling of axes by a function (GraphToolFunctor).
/// Author: Blair Jamieson (Feb 2017)
/// 
///=========================================================================
/// GraphToolFunctor
/// ---------------
/// Allow scaling of axes in GraphTool
/// To scale axes, need to define your own functor that
/// inherits from the GraphToolFunctor.
struct GraphToolFunctor {
  GraphToolFunctor(){ ; }
  virtual double operator()( const double ) = 0;
  virtual ~GraphToolFunctor(){;};
};


///=========================================================================
/// GraphTool
/// ---------
/// Class to read table of x, y values from text file
/// Contructor takes:
/// 1) filename to read in:
///   file should be formatted with comment lines with # in first char
///   followed by two columns of data
/// 2) Unit name for x-axis as std::string
/// 3) Unit name for y-axis as std::string
/// 4) pointer to a GraphToolFunctor or nullptr -- GraphTool now owns that memory
/// 5) pointer to a GraphtoolFunctor or nullptr -- GraphTool now owns that memory
struct GraphTool{
  GraphTool( std::string afname, std::string axname, std::string ayname,
	     GraphToolFunctor * xfcn, GraphToolFunctor * yfcn );
  virtual ~GraphTool(){
    if ( fcnx != nullptr ) delete fcnx;
    if ( fcny != nullptr ) delete fcny;
  }
  TGraph * get_TGraph() const { return ftg; }

protected:
  virtual void parse_file();
  std::string fname;
  std::string xname;
  std::string yname;
  TGraph * ftg;
  GraphToolFunctor* fcnx;
  GraphToolFunctor* fcny;
};


//=========================================================================
// print table from GraphTool's TGraph
std::ostream & operator<<( std::ostream &os, const GraphTool & d );

#endif
