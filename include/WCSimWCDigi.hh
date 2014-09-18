#ifndef WCSimWCDigi_h
#define WCSimWCDigi_h 1

#include "G4VDigi.hh"
#include "G4TDigiCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4ios.hh"

#include <set>
#include <map>
#include <vector>

// for sort, find, count_if
#include <algorithm>
//for less_equal, bind2nd,...
#include <functional>

// compose2 is not part of the C++ standard
// use this kludgy technique to use it
#include <ext/functional>
using __gnu_cxx::identity_element;
using __gnu_cxx::unary_compose;
using __gnu_cxx::binary_compose;
using __gnu_cxx::compose1;
using __gnu_cxx::compose2;
using __gnu_cxx::identity;
using __gnu_cxx::select1st;
using __gnu_cxx::select2nd;
using __gnu_cxx::project1st;
using __gnu_cxx::project2nd;
using __gnu_cxx::constant_void_fun;
using __gnu_cxx::constant_unary_fun;
using __gnu_cxx::constant_binary_fun;
using __gnu_cxx::constant0;
using __gnu_cxx::constant1;
using __gnu_cxx::constant2;
using __gnu_cxx::subtractive_rng;
using __gnu_cxx::mem_fun1;
using __gnu_cxx::mem_fun1_ref;

class WCSimWCDigi : public G4VDigi
{

public:
  
  WCSimWCDigi();
  ~WCSimWCDigi();
  WCSimWCDigi(const WCSimWCDigi&);
  const WCSimWCDigi& operator=(const WCSimWCDigi&);
  int operator==(const WCSimWCDigi&) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  
  void Draw();
  void Print();

private:
  
  G4int   tubeID;
  std::set<int> Gates; // list of gates that were hit  
  std::vector<float> TriggerTimes;
  std::map<int,float> pe;   
  std::map<int,float> time; 

  G4double         edep;
  G4ThreeVector    pos;
  G4LogicalVolume* pLogV;
  static G4int     maxPe;

  G4int                 totalPe;
  std::vector<G4float>  time_float;
  std::vector<G4int>    primaryParentID;
  G4int                 totalPeInGate;
  G4RotationMatrix rot;
  G4int            trackID;

public:
  
  inline void SetTubeID(G4int tube) {tubeID = tube;};
  inline void AddGate(int g,float t) 
  { Gates.insert(g); TriggerTimes.push_back(t);}
  inline void SetPe(G4int gate,  G4float Q)      {pe[gate]     = Q;};
  inline void SetTime(G4int gate, G4float T)    {time[gate]   = T;};
  
  inline G4float GetGateTime(int gate) { return TriggerTimes[gate];}
  inline G4int   GetTubeID() {return tubeID;};
  inline G4float GetPe(int gate)     {return pe[gate];};
  inline G4float GetTime(int gate)   {return time[gate];};
  inline int NumberOfGates() { return Gates.size();}
  inline int NumberOfSubEvents() { return (Gates.size()-1);}
  inline bool HasHitsInGate(int number) { return (Gates.count(number)==1); }

  G4LogicalVolume* GetLogicalVolume() {return pLogV;};


  void SetEdep         (G4double de)                { edep = de; };
  void SetPos          (G4ThreeVector xyz)          { pos = xyz; };
  void SetLogicalVolume(G4LogicalVolume* logV)      { pLogV = logV;}
  void SetTrackID      (G4int track)                { trackID = track; };
  void AddParentID     (G4int primParentID)
  { primaryParentID.push_back(primParentID); }
  void SetRot          (G4RotationMatrix rotMatrix) { rot = rotMatrix; };
  G4int         GetTotalPe()    { return totalPe;};

  void SetMaxPe(G4int number = 0)  {maxPe   = number;};

  void AddPe(G4float hitTime)  
  {
    // Increment the totalPe number
    totalPe++; 
        
    time_float.push_back(hitTime);
  }

  //  void SortHitTimes() {   sort(time_float.begin(),time_float.end()); }
 void SortHitTimes() {   sort(time_float.begin(),time_float.end()); }

  G4float GetFirstHitTimeInGate(G4float low,G4float upevent)
  {
    G4float firsttime;
    std::vector<G4float>::iterator tfirst = time_float.begin();
    std::vector<G4float>::iterator tlast = time_float.end();
    
    std::vector<G4float>::iterator found = 
      std::find_if(tfirst,tlast,
		   compose2(std::logical_and<bool>(),
			    std::bind2nd(std::greater_equal<G4float>(),low),
			    std::bind2nd(std::less_equal<G4float>(),upevent)
			    )
		   );
    if ( found != tlast ) {
      firsttime = *found; // first hit time
    }
    else {
      firsttime = -10000.; //error code.
    }
    //G4cout << "firsthit time " << firsttime << "\n";
    return firsttime;
  }

 G4int GetPeInGate(double low, double pmtgate,double evgate) {
    // M Fechner; april 2005
    // assumes that time has already been sorted
    std::vector<G4float>::iterator tfirst = time_float.begin();
    std::vector<G4float>::iterator tlast = time_float.end();
    // select min time
    G4float mintime = (pmtgate < evgate) ? pmtgate : evgate;
    
    // return number of hits in the time window...
    
    G4int number = std::count_if(tfirst,tlast,
				 compose2(std::logical_and<bool>(),
					  std::bind2nd(std::greater_equal<G4float>(),low),
					  std::bind2nd(std::less_equal<G4float>(),mintime)
					  )
				 );
    
    totalPeInGate = number;
    //    G4cout << "numer = " <<  number <<"\n";
    return number;
  }


};

typedef G4TDigiCollection<WCSimWCDigi> WCSimWCDigitsCollection;
extern G4Allocator<WCSimWCDigi> WCSimWCDigiAllocator;

inline void* WCSimWCDigi::operator new(size_t)
{
  void* aDigi;
  aDigi = (void*) WCSimWCDigiAllocator.MallocSingle();
  return aDigi;
}

inline void WCSimWCDigi::operator delete(void* aDigi)
{
  WCSimWCDigiAllocator.FreeSingle((WCSimWCDigi*) aDigi);
}

#endif









