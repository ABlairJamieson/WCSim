#ifndef WCSimLEDMessenger_h
#define WCSimLEDMessenger_h 1

class WCSimPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWith3Vector;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

#include <iostream>
#include "LEDTimeDistro.hh"
#include "G4UImessenger.hh"
#include "globals.hh"



/// WCSIMLEDMessenger
/// Class to hold settings for light emitting diode source of photons
/// that can be set from the Geant4 user interface in the /led directory.
/// Use /gps commands to set the LED Energy, position, and direction.
/// /led directory for time structure
///
/// Settings include:
///   5) time distribution of source
///
/// Author: Blair Jamieson (Feb, 2017)
class WCSimLEDMessenger: public G4UImessenger
{
 public:
  WCSimLEDMessenger();
  ~WCSimLEDMessenger();

  // standard G4UImessenger callbacks
  void     SetNewValue(G4UIcommand* command, G4String newValues);
  G4String GetCurrentValue(G4UIcommand* command);

  // method to get a random time following requested
  // distribution
  double GetTime();
  
 private:
  
  // geant4 user interface commands
  /// ledDir: directory holding all led source settings
  G4UIdirectory*       ledDirectory;

  /// ledNsTypeCmd: time distribution type 0: uniform min-max, 1:
  /// gaussian with mean and sigma in time 2 (or above): file of
  /// time (ns), intensity
  G4UIcmdWithAnInteger*     ledNsTypeCmd;

  /// ledNsMinCmd: min time in ns
  G4UIcmdWithADouble*  ledNsMinCmd;

  /// ledNsMaxCmd: max time in ns
  G4UIcmdWithADouble*  ledNsMaxCmd;

  /// ledNsFileCmd: filename of time distribution of source textfile
  /// of time(ns) and intensity
  G4UIcmdWithAString*  ledNsFileCmd;

  /// ledNsMinCmd: min time in ns
  G4UIcmdWithADouble*  ledNsMeanCmd;

  /// ledNsMaxCmd: max time in ns
  G4UIcmdWithADouble*  ledNsSigmaCmd;

  // Store values set from user interface
  int         ledNsType;
  double      ledNsMin;
  double      ledNsMax;
  double      ledNsMean;
  double      ledNsSigma;
  std::string ledNsFile;

  LEDTimeDistro * fLedTime;
};

#endif


