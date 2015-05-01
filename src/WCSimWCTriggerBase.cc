#include "WCSimWCTriggerBase.hh"
#include "WCSimWCPMT.hh"
#include "WCSimWCDigi.hh"
#include "WCSimWCHit.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4DigiManager.hh"
#include "G4ios.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"

#include "WCSimDetectorConstruction.hh"
#include "WCSimPmtInfo.hh"
#include "WCSimDarkRateMessenger.hh"

#include <vector>
// for memset
#include <cstring>
#include <iostream>


const double WCSimWCTriggerBase::offset = 950.0 ; // ns
const double WCSimWCTriggerBase::pmtgate = 200.0 ; // ns. integration time
const double WCSimWCTriggerBase::eventgateup = 950.0 ; // ns. save eventgateup ns after the trigger time
const double WCSimWCTriggerBase::eventgatedown = -400.0 ; // ns. save eventgateup ns before the trigger time
const double WCSimWCTriggerBase::LongTime = 100000.0 ; // ns = 0.1ms. event time


WCSimWCTriggerBase::WCSimWCTriggerBase(G4String name,
				   WCSimDetectorConstruction* myDetector)
  :G4VDigitizerModule(name)
{
  G4String colName = "WCDigitizedCollection";
  this->myDetector = myDetector;
  collectionName.push_back(colName);
  DigiHitMap.clear();
  TriggerTimes.clear();
}

WCSimWCTriggerBase::~WCSimWCTriggerBase(){
}

void WCSimWCTriggerBase::Digitize()
{
  //Input is collection of all digitized hits that passed the threshold
  //Output is all digitized hits which pass the trigger
  
  DigiHitMap.clear();
  TriggerTimes.clear();

  //This is the output digit collection
  DigitsCollection = new WCSimWCDigitsCollection ("/WCSim/glassFaceWCPMT",collectionName[0]);

  G4DigiManager* DigiMan = G4DigiManager::GetDMpointer();

  // Get the Digitized hits collection ID
  G4int WCDCID = DigiMan->GetDigiCollectionID("WCDigitizedStoreCollection");
  // Get the PMT Digits Collection
  WCSimWCDigitsCollection* WCDCPMT = 
    (WCSimWCDigitsCollection*)(DigiMan->GetDigiCollection(WCDCID));

  // Do the work  
  if (WCDCPMT) {
    DoTheWork(WCDCPMT);
  }
  
  StoreDigiCollection(DigitsCollection);
}

void WCSimWCTriggerBase::AlgNHits(WCSimWCDigitsCollection* WCDCPMT, bool remove_hits) {


  //Now we will try to find triggers
  //loop over PMTs, and Digits in each PMT.  If nhits > Threshhold in a time window, then we have a trigger

  int ntrig = 0;
  int window_start_time = 0;
  WCSimPMTObject * PMT = myDetector->GetPMTPointer("glassFaceWCPMT"); //for hit time smearing

  //float lower=0;
  //float upper = WCSimWCDigitizerSKIV::pmtgate;
  G4cout << "NUMBER OF ENTRIES IN DIGIT COLLECTION " << WCDCPMT->entries() << G4endl;

  // the upper time limit is set to the final possible full trigger window
  while(window_start_time <= (WCSimWCDigitizerSKIV::LongTime - WCSimWCDigitizerSKIV::pmtgate)) {
    int n_digits = 0;
    //float firstinwindow = WCSimWCDigitizerSKIV::LongTime + 1; //a time that is not possible for a hit
    float triggertime; //save each digit time, because the trigger time is the time of the first hit above threshold
    bool triggerfound = false;
    vector<int> digit_times;
    //Loop over each PMT
    for (G4int i = 0 ; i < WCDCPMT->entries() ; i++) {
      int tube=(*WCDCPMT)[i]->GetTubeID();
      //Loop over each Digit in this PMT
      for ( G4int ip = 0 ; ip < (*WCDCPMT)[i]->GetTotalPe() ; ip++) {
	int digit_time = (*WCDCPMT)[i]->GetTime(ip);
	//hit in trigger window?
	if(digit_time >= window_start_time && digit_time <= (window_start_time + WCSimWCDigitizerSKIV::pmtgate)) {
	  n_digits++;
	  digit_times.push_back(digit_time);
	  //if(firstinwindow > digit_time)
	  //firstinwindow = digit_time;
	}
      }//loop over Digits
    }//loop over PMTs
    
    //if over threshold, issue trigger
    if(n_digits > nhitsThreshold) {
      ntrig++;
      //The trigger time is the time of the first hit above threshold
      std::sort(digit_times.begin(), digit_times.end());
      triggertime = digit_times[nhitsThreshold];
      //triggertime = firstinwindow;
      TriggerTimes.push_back(triggertime);
      TriggerTypes.push_back(kNHits);
      triggerfound = true;
    }

    //move onto the next go through the timing loop
    if(triggerfound) {
      window_start_time = triggertime + WCSimWCDigitizerSKIV::eventgateup;
    }//triggerfound
    else {
      window_start_time += 10;
    }
  }
  
  //call FillDigitsCollection() if at least one trigger was issued
  if(ntrig)
    FillDigitsCollection(WCDCPMT, remove_hits);
}

void WCSimWCTriggerBase::FillDigitsCollection(WCSimWCDigitsCollection* WCDCPMT, bool remove_hits)
{
  //Loop over triggers
  for(int itrigger = 0; itrigger < TriggerTimes.size(); itrigger++) {
    triggertime = TriggerTimes[itrigger];
    triggertype = TriggerTypes[itrigger];
    float lowerbound = triggertime + WCSimWCDigitizerSKIV::eventgatedown;
    float upperbound = triggertime + WCSimWCDigitizerSKIV::eventgateup;

    //loop over PMTs
    for (G4int i = 0; i < WCDCPMT->entries(); i++) {
      int tube=(*WCDCPMT)[i]->GetTubeID();
      //loop over digits
      for ( G4int ip = 0; ip < (*WCDCPMT)[i]->GetTotalPe(); ip++){
	int digit_time  = (*WCDCPMT)[i]->GetTime(ip);
	float peSmeared = (*WCDCPMT)[i]->GetPe(ip);
	//int parentID    = (*WCDCPMT)[i]->GetParentID(ip);
	if(digit_time >= lowerbound && digit_time <= upperbound) {
	  //hit in event window
	  //add to DigitsCollection
	  float Q = (peSmeared > 0.5) ? peSmeared : 0.5;
	  G4double digihittime = -triggertime
	    + WCSimWCDigitizerSKIV::offset
	    + digit_time
	    + PMT->HitTimeSmearing(Q);
	  //add hit
	  if ( DigiHitMap[tube] == 0) {
	    WCSimWCDigi* Digi = new WCSimWCDigi();
	    Digi->AddParentID(1); //parentID
	    Digi->SetTubeID(tube);
	    Digi->AddGate  (itrigger,triggertime);
	    Digi->SetPe    (itrigger,peSmeared);
	    Digi->AddPe    (digihittime);
	    Digi->SetTime  (itrigger,digihittime);
	    //Digi->SetTriggerID(triggertype);
	    DigiHitMap[tube] = DigitsCollection->insert(Digi);
	  }
	  else {
	    //(*DigitsCollection)[DigiHitMap[tube]-1]->AddParentID(parentID);
	    (*DigitsCollection)[DigiHitMap[tube]-1]->AddGate(itrigger, triggertime);
	    (*DigitsCollection)[DigiHitMap[tube]-1]->SetPe  (itrigger, peSmeared);
	    (*DigitsCollection)[DigiHitMap[tube]-1]->SetTime(itrigger, digihittime);
	    (*DigitsCollection)[DigiHitMap[tube]-1]->AddPe  (digihittime);
	    //(*DigitsCollection)[DigiHitMap[tube]-1]->SetTriggerID(triggertype);
	  }
	}//digits within trigger window
      }//loop over Digits
    }//loop over PMTs
  }//loop over Triggers
}
