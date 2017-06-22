
// make time distribution file with "capacitor charging" and "discharging" profile
// parameters as follows.
// Turn on time will be 3*trise
// Turn off time will be 3*tfall
// Total time = 3*trise + ontime + 3*tfall
void maketimedistro(int npoints = 100,
		    double trise = 5.0, //rise-time in ns
		    double tfall = 15.0, //fall-time in ns
		    double ontime = 10.0, // time that led is "on" in ns  
		    char * filename = "timedistro" ){
  std::vector<double> time;
  std::vector<double> intensity;

  
  double totaltime = 3*trise + ontime + 3*tfall;
  double dt = totaltime / npoints;
  
  double curtime=0.;
  for (int i=0; i<npoints; ++i){
    if ( curtime < 3*trise ){
      // turning on
      time.push_back( curtime );
      intensity.push_back( 1.0-exp( -curtime / trise ) );
			   
    } else if ( curtime < 3*trise + ontime ) {
	// on
      time.push_back( curtime );
      intensity.push_back( 1.0-exp( -3.0 ) );
			   
    } else {
      // turning off
      double offtime = curtime - (3*trise+ontime);
      time.push_back( curtime );
      intensity.push_back( (1.0-exp(-3.0))*exp(-offtime/tfall) );
    }
    
    curtime += dt;
  }
  

  // check result
  TGraph * tg = new TGraph( npoints, &time[0], &intensity[0] );
  tg->SetMarkerStyle(20);
  tg->Draw("ALP");

  // write output to file
  char outfilename[256];
  sprintf( outfilename, "%s_trise%f_tfall%f_on%f.txt",filename, trise, tfall, ontime);
  ofstream fout( outfilename, std::ofstream::out);
  for (unsigned ii=0; ii<time.size(); ++ii){
    fout << time[ii] <<" "<<intensity[ii] << std::endl;
  }
  // fout.write();
  fout.close();

}
