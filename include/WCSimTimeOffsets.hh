#ifndef _WCSimTimeOffsets_hh_
#define _WCSimTimeOffsets_hh_

#include <map>
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADouble;                                                          
class G4UIcmdWithAnInteger;

/// WCSimTimeOffsets
///
/// Simple messenger class to hold settings for what time offests per
/// channel to apply.  Stores offsets as std::map< int, double > where key
/// in map is pmtid, and double is the time offset in ns.
/// Settings for time offsets are stored in:
///  /timeoffs/type 0 -- disabled, 1 -- use input file, 2 -- use gaussian mean, sigma
///  /timeoffs/file "filename.txt"  file has two columns: pmtid, time offset
///  /timeoffs/mean 0.0
///  /timeoffs/sigma 10.0
///
/// Author: Blair Jamieson (Jun, 2017)
class WCSimTimeOffsets : public G4UImessenger {
public:

  /// Helper function to print map
  friend std::ostream & operator<<( std::ostream & out, const WCSimTimeOffsets& wto );
  
  WCSimTimeOffsets();
  ~WCSimTimeOffsets();

 
  /// standard G4UImessenger callbacks
  void     SetNewValue(G4UIcommand* command, G4String newValues);
  G4String GetCurrentValue(G4UIcommand* command);
  
  /// method to get a time offset
  double GetTimeOffset( int pmtid );
  
  /// method to over-ride number of pmts
  /// used mainly in case of mean and sigma random offsets
  void SetNPMTs( int npmt ){ 
    std::cout<<"WCSimTimeOffsets::SetNPMTs = "<<npmt<<std::endl;
    toffsNPMT = npmt; 
  }

  
 private:


  /// method to initialize pmt offsets map
  void Init();
  // geant4 user interface commands
  /// toffsDir: directory holding time offset settings
  G4UIdirectory*       toffsDir;
  
  /// toffsType
  /// Type of time offset (0==disabled, 1==use file, 2==use mean and sigma)
  G4UIcmdWithAnInteger* toffsTypeCmd;

  /// toffsFileNameCmd
  G4UIcmdWithAString * toffsFileNameCmd;
  
  /// toffsMeanCmd: min time in ns
  G4UIcmdWithADouble*  toffsMeanCmd;

  /// toffsSigmaCmd: max time in ns
  G4UIcmdWithADouble*  toffsSigmaCmd;

  /// Store values set from user interface
  int toffsType;
  std::string toffsFile;
  double toffsMean;
  double toffsSigma;
  int toffsNPMT;

protected:
  /// Map of pmtid and offset
  std::map< int, double > toffs;
 
};




#endif //_WCSimTimeOffsets_hh_
