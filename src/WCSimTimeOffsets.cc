
#include <fstream>
#include <iterator>

#include "WCSimTimeOffsets.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "TRandom.h"


WCSimTimeOffsets::WCSimTimeOffsets() :
  toffsType(0), toffsFile(""), toffsMean(0.), toffsSigma(1.){

  toffsDir = new G4UIdirectory( "/toffs/" );
  toffsDir->SetGuidance("PMT Time offset per PMT control commands");

  toffsTypeCmd = new G4UIcmdWithAnInteger( "/toffs/type", this );
  toffsTypeCmd->SetGuidance("Type of offset: 0==none (default), 1==file, 2==mean,sigma");
  toffsTypeCmd->SetDefaultValue( toffsType );

  toffsFileNameCmd = new G4UIcmdWithAString( "/toffs/file", this );
  toffsFileNameCmd->SetGuidance("Filename of text file with two columns: pmtid and time offset (ns)");
  toffsFileNameCmd->SetDefaultValue( toffsFile.c_str() );

  toffsMeanCmd = new G4UIcmdWithADouble( "/toffs/mean", this );
  toffsMeanCmd->SetGuidance("Mean offset for gaussain case (0. default)");
  toffsMeanCmd->SetDefaultValue( toffsMean );

  toffsSigmaCmd = new G4UIcmdWithADouble( "/toffs/sigma", this );
  toffsSigmaCmd->SetGuidance("Sigma offset for gaussain case (1. default)");
  toffsSigmaCmd->SetDefaultValue( toffsSigma );

}

WCSimTimeOffsets::~WCSimTimeOffsets(){
  if ( toffsDir ) delete toffsDir;
  if ( toffsTypeCmd ) delete toffsTypeCmd;
  if ( toffsFileNameCmd ) delete toffsFileNameCmd;
  if ( toffsMeanCmd ) delete toffsMeanCmd;
  if ( toffsSigmaCmd ) delete toffsSigmaCmd;
}

void WCSimTimeOffsets::SetNewValue( G4UIcommand * cmd, G4String val ){

  if ( cmd == toffsTypeCmd ){
    toffsType = toffsTypeCmd->GetNewIntValue( val );
    G4cout << "WCSimTimeOffsets::SetNewValue /toffs/type set to "<<toffsType<< G4endl;
    return;
  }

  if ( cmd == toffsFileNameCmd ){
    toffsFile = std::string( val );
    G4cout << "WCSimTimeOffsets::SetNewValue /toffs/file set to "<<toffsFile<< G4endl;
    return;
  }

  if ( cmd == toffsMeanCmd ){
    toffsMean = toffsMeanCmd->GetNewDoubleValue( val );
    G4cout << "WCSimTimeOffsets::SetNewValue /toffs/mean set to "<<toffsMean<< G4endl;
    return;
  }

  if ( cmd == toffsSigmaCmd ){
    toffsSigma = toffsSigmaCmd->GetNewDoubleValue( val );
    G4cout << "WCSimTimeOffsets::SetNewValue /toffs/sigma set to "<<toffsSigma<< G4endl;
    return;  
  }
  
}



//=========================================================================
G4String WCSimTimeOffsets::GetCurrentValue( G4UIcommand * cmd ){
  G4String cv = "";
  return cv;
}


double WCSimTimeOffsets::GetTimeOffset( int pmtid ){

  // only initialize times once:
  if ( toffs.size() == 0 ) Init();

  std::map< int, double >::iterator offset = toffs.find( pmtid ); 
  if ( offset != toffs.end() ){
    return offset->second;
  } else {
    G4cout <<"WCSimTimeOffset::GetTimeOffset( "
	   << pmtid <<" pmtid not in table, returning 0 offset"
	   << G4endl;
  }
  return 0.0;
}


void WCSimTimeOffsets::Init(){
  std::cout<<"WCSimTimeOffsets::Init"<<std::endl;
  if ( toffsType == 1 ){
    // read input file
    std::cout<<"WCSimTimeOffsets::Init with offset file"<<std::endl;
    if ( toffsFile.size() == 0 ) {
      std::cout<<"WCSimTimeOffsets::Init (Warning) no time offset file provided"<<std::endl;
   
      return;
    }
    std::ifstream fin( toffsFile.c_str() );
    if (!fin) std::cout<<"WCSimtimeOffsets::Init (Warning) "
	      " could not open file: "<<toffsFile<<std::endl;
    int ipmt;
    double timeoffset;
    std::string s;
    // ignore comment lines
    while ( fin && fin.peek() == '#' ) getline( fin, s );
    while ( fin && fin >> ipmt >> timeoffset ){
      toffs[ ipmt ] = timeoffset;
    }
  }

  if ( toffsType == 2 ){
    // set values randomly
    std::cout<<"WCSimTimeOffsets::Init with random gaussian "
	     <<" mean="<<toffsMean<<" sigma="<<toffsSigma<<std::endl;
    
    for ( int ipmt=0; ipmt<toffsNPMT; ++ipmt ){
      toffs[ ipmt ] = gRandom->Gaus( toffsMean, toffsSigma );
    }
  }

  std::cout << *this << std::endl;

  return;
}

std::ostream & operator<<( std::ostream & out,
			   const WCSimTimeOffsets& wto ){
  out <<"WCSimTimeOFfsets, pmt map: ipmt  timeoffset" << std::endl;
  std::map< int, double >::const_iterator ito = wto.toffs.begin();
  while ( ito != wto.toffs.end() ){
    out<< ito->first<<"  "<<ito->second<<" ns"<<std::endl;
    ++ito;
  }
  return out;
}
