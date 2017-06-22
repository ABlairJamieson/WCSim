#ifndef LEDTimeDistro_h
#define LEDTimeDistro_h 1

#include "GraphTool.hh"
#include <string>

/// LEDTimeDistro
/// Class to hold form for time distribution of photons from LED pulser
/// Author: Blair Jamieson (Feb. 2017)
struct LEDTimeDistro : GraphTool {
  /// Construct uniform distribution between atmin and atmax
  /// times in ns
  LEDTimeDistro( double atmin, double atmax );
  /// Construct distribution following file afname that is a text
  /// file with two columns: time, and intensity
  LEDTimeDistro( std::string afname );

  /// Get a random time that follows distribution
  double random_time() const;

private:
  double ftmin;
  double ftmax;
};

#endif
