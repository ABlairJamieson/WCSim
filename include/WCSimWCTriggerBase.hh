#ifndef WCSimWCTriggerBase_h
#define WCSimWCTriggerBase_h 1

#include "WCSimEnumerations.hh"
#include "WCSimWCDAQMessenger.hh"
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

  WCSimWCTriggerBase(G4String name, WCSimDetectorConstruction*, WCSimWCDAQMessenger*);
  ~WCSimWCTriggerBase();
  
  //not recommended to override this method
  virtual void Digitize(); //only defined virtual because it is overridden in the old class (WCSimWCDigitizer)

  void ReInitialize() {
    TriggerTimes.clear(); 
    TriggerTypes.clear(); 
    TriggerInfos.clear(); 
    DigiHitMap.clear();
  }
  int NumberOfGatesInThisEvent() { return TriggerTimes.size(); }
  G4double      GetTriggerTime(int i) { return TriggerTimes[i];}
  TriggerType_t GetTriggerType(int i) { return TriggerTypes[i];}
  G4double      GetTriggerInfo(int i) { return TriggerInfos[i];}
  void SetNHitsThreshold(G4int threshold) { nhitsThreshold = threshold; }
  void SetPMTSize(G4float inputSize) { PMTSize = inputSize; }

protected:

  virtual void DoTheWork(WCSimWCDigitsCollection* WCDCPMT) = 0; //this should call the trigger algorithms, and handle any temporary DigitsCollection's
  //these are the algorithms that perform triggering
  void AlgNHits(WCSimWCDigitsCollection* WCDCPMT, bool remove_hits);

  WCSimWCDigitsCollection*   DigitsCollection;
  std::map<int,int>          DigiHitMap; // need to check if a hit already exists..

  std::vector<G4double>      TriggerTimes;
  std::vector<TriggerType_t> TriggerTypes;
  std::vector<G4double>      TriggerInfos;

  WCSimWCDAQMessenger*       DAQMessenger;
  WCSimDetectorConstruction* myDetector;

private:

  G4float PMTSize;
  G4int nhitsThreshold;

  //takes all trigger times, then loops over all Digits & fills the output DigitsCollection
  void FillDigitsCollection(WCSimWCDigitsCollection* WCDCPMT, bool remove_hits);
  
  



  
  //the following data members will be made into parameters, and possibly renamed
private:
  static const double offset; // hit time offset
  static const double pmtgate; // ns
  static const double eventgateup; // ns
  static const double eventgatedown; // ns
  static const double LongTime; // ns


};

#endif








