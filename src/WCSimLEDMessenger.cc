#include "WCSimLEDMessenger.hh"
#include "TRandom.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "TRandom.h"


//=========================================================================
WCSimLEDMessenger::WCSimLEDMessenger() :
  ledNsType(0),
  ledNsMin(0),
  ledNsMax(0),
  ledNsMean(0),
  ledNsSigma(0),
  ledNsFile(""),
  fLedTime( NULL )
{
  
  ledDirectory = new G4UIdirectory( "/led/" );
  ledDirectory->SetGuidance( "WCSim led photon source control commands." );

  ledNsTypeCmd = new G4UIcmdWithAnInteger ( "/led/NsType", this );
  ledNsTypeCmd->SetGuidance("Time distribution type 0: uniform min-max, 1:mean and sigma, 2+: file of time(ns), intensity" );
  ledNsTypeCmd->SetDefaultValue( ledNsType );

  ledNsMinCmd = new G4UIcmdWithADouble ( "/led/NsMin", this );
  ledNsMinCmd->SetGuidance("Min time for photons from led");
  ledNsMinCmd->SetDefaultValue( ledNsMin );

  ledNsMaxCmd = new G4UIcmdWithADouble ( "/led/NsMax", this );
  ledNsMaxCmd->SetGuidance("Max time for photons from led");
  ledNsMaxCmd->SetDefaultValue( ledNsMax );

  ledNsMeanCmd = new G4UIcmdWithADouble ( "/led/NsMean", this );
  ledNsMeanCmd->SetGuidance("Mean time for photons from led");
  ledNsMeanCmd->SetDefaultValue( ledNsMean );

  ledNsSigmaCmd = new G4UIcmdWithADouble ( "/led/NsSigma", this );
  ledNsSigmaCmd->SetGuidance("Sigma time for photons from led");
  ledNsSigmaCmd->SetDefaultValue( ledNsSigma );

  ledNsFileCmd = new G4UIcmdWithAString ( "/led/NsFile", this );
  ledNsFileCmd->SetGuidance("Time distribuition file name.");
  ledNsFileCmd->SetDefaultValue("");

}


//=========================================================================
WCSimLEDMessenger::~WCSimLEDMessenger(){
  if (ledDirectory)  delete ledDirectory;
  if (ledNsTypeCmd)  delete ledNsTypeCmd;
  if (ledNsMinCmd)   delete ledNsMinCmd;
  if (ledNsMaxCmd)   delete ledNsMaxCmd;
  if (ledNsMeanCmd)   delete ledNsMeanCmd;
  if (ledNsSigmaCmd)   delete ledNsSigmaCmd;
  if (ledNsFileCmd)  delete ledNsFileCmd;
  if (fLedTime)           delete fLedTime;  
}

//=========================================================================
void WCSimLEDMessenger::SetNewValue( G4UIcommand * cmd, G4String val ){

  if ( cmd == ledNsTypeCmd ){
    ledNsType = ledNsTypeCmd->GetNewIntValue( val );
    G4cout << "Led wavelength type set to "<< ledNsType << G4endl;
    return;    
  }
  if ( cmd == ledNsMinCmd ){
    ledNsMin = ledNsMinCmd->GetNewDoubleValue( val );
    G4cout << "Led min time set to "
	   << ledNsMin << " nm " << G4endl;
    return;    
  }
  if ( cmd == ledNsMaxCmd ){
    ledNsMax = ledNsMaxCmd->GetNewDoubleValue( val );
    G4cout << "Led max time set to "
	   << ledNsMax << " nm " << G4endl;
    return;    
  }  
  if ( cmd == ledNsMeanCmd ){
    ledNsMean = ledNsMeanCmd->GetNewDoubleValue( val );
    G4cout << "Led mean time set to "
	   << ledNsMean << " nm " << G4endl;
    return;    
  }
  if ( cmd == ledNsSigmaCmd ){
    ledNsSigma = ledNsSigmaCmd->GetNewDoubleValue( val );
    G4cout << "Led sigma time set to "
	   << ledNsSigma << " nm " << G4endl;
    return;    
  }  
  if (  cmd == ledNsFileCmd ){
    ledNsFile = std::string( val );
    G4cout << "Led time distribution file set to "<< ledNsFile << G4endl;
    return;
  }
  
}

//=========================================================================
G4String WCSimLEDMessenger::GetCurrentValue( G4UIcommand * cmd ){
  G4String cv = "";
  return cv;
}


double WCSimLEDMessenger::GetTime() {

  if ( ledNsType == 0 ){
    // generate between min and max time
    return gRandom->Rndm()*(ledNsMax - ledNsMin) + ledNsMin;
  } else if ( ledNsType == 1 ) {
    // generate random number following gaussian
    return gRandom->Gaus( ledNsMean, ledNsSigma );
  } else {
    // any other number assume a file
    if ( fLedTime == NULL ){
      fLedTime = new LEDTimeDistro( ledNsFile );
    }
    double thistime = fLedTime->random_time();
    //std::cout<<"WCSimLEDMessenger::GetTime = "<<thistime<<std::endl;
    return thistime;
  }

  // should never get here
  return 0.;
}
