#ifndef WCSimWCTriggerBase_h
#define WCSimWCTriggerBase_h 1

#include "WCSimDarkRateMessenger.hh"
#include "WCSimDetectorConstruction.hh"
#include "G4VDigitizerModule.hh"
#include "WCSimWCDigi.hh"
#include "WCSimWCHit.hh"
#include "globals.hh"
#include "Randomize.hh"
#include <map>
#include <vector>


class WCSimWCTriggerBase : public G4VDigitizerModule
{
public:

  WCSimWCTriggerBase(G4String name, WCSimDetectorConstruction*);
  ~WCSimWCTriggerBase();
  
  //not recommended to override these methods
  virtual void Digitize(); //only defined virtual because it is overridden in the old class (WCSimWCDigitizer)

  int NumberOfGatesInThisEvent() { return TriggerTimes.size(); }
  G4double GetTriggerTime(int i) { return TriggerTimes[i];}
  void SetPMTSize(G4float inputSize) {PMTSize = inputSize;}

private:
  virtual void ApplyTrigger(WCSimWCDigitsCollection* WCDCPMT) = 0;
  virtual void DoTheWork   (WCSimWCDigitsCollection* WCDCPMT); // could be overridden in the implementation classes
                                                               //  by default just calls ApplyTrigger()

  WCSimDetectorConstruction* myDetector;
  std::vector<G4double> TriggerTimes;
  G4float PMTSize;

protected:
  WCSimWCDigitsCollection*  DigitsCollection;


  //the following methods & data members may be removed
  
public:
  void ReInitialize() { DigiHitMap.clear(); }
  void AddNewDigit(int tube, int gate, float digihittime, float peSmeared);
  void DigitizeHits(WCSimWCDigitsCollection* WCHCPMT);
  void DigitizeGate(WCSimWCDigitsCollection* WCHC,G4int G);

  static G4double GetLongTime() { return LongTime;}
  static G4double GetEventGateDown() { return eventgatedown;}
  static G4double GetEventGateUp() { return eventgateup;}

private:
  static const double offset; // hit time offset
  static const double pmtgate; // ns
  static const double eventgateup; // ns
  static const double eventgatedown; // ns
  static const double calibdarknoise; // ns
  static const double LongTime; // ns
  static const int GlobalThreshold; //number of hit PMTs within an <=200ns sliding window that decides the global trigger t0

  G4float RealOffset;  // t0 = offset corrected for trigger start
  G4float MinTime;  // very first hit time



protected:
  std::map<int,int> DigiHitMap; // need to check if a hit already exists..

};

#endif








