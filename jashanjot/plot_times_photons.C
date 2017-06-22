
struct Tracks_struct {
  //tracks:
  int nTracks;             // number of final state particles
  int pid[100];             // id of ith final state particle
  int flag[100];            // flag: -1 = beam particle (incoming neutrino)
                           //       -2 = target
                           //        0 = pGun or Cosmic primary
                           //        1 = outgoing lepton
                           //        2 = most energetic outgoing nucleon
  float mass[100];          // mass of ith final state particle
  float p[100];             // momentum of ith final state particle
  float energy[100];        // energy of ith final state particle
  int parentid[100];      // parent id
  int trackid[100];      // track id
  float time[100];      // track time
  float dir_x[100];     // direction of ith final state particle
  float dir_y[100];     // direction of ith final state particle
  float dir_z[100];     // direction of ith final state particle
  float pdir_x[100];    // momentum-vector of ith final state particle
  float pdir_y[100];    // momentum-vector of ith final state particle
  float pdir_z[100];    // momentum-vector of ith final state particle
  float start_x[100];   // starting point of ith final state particle
  float start_y[100];   // starting point of ith final state particle
  float start_z[100];   // starting point of ith final state particle
  float stop_x[100];    // stopping point of ith final state particle
  float stop_y[100];    // stopping point of ith final state particle
  float stop_z[100];    // stopping point of ith final state particle
  float length[100];    // TrackLength

};


struct Cherenkovhits_struct {
  int   run;                      // run number
  int   event;                    // event number
  int   subevent;                 // sub event number           
  int   numTubesHit;              //  Total number of tubes with hits
  int   numTubesHit_noNoise;      // Total number of tubes with physics hits
  int   totalNumHits;             // hits per event = hits per Tube x numTubesHit
  int   totalNumHits_noNoise;     // hits per event = hits per Tube x numTubesHit
  int   totalPe[10000000];        // The totalPE recorded at each tube
  int   totalPe_noNoise[10000000];// The totalPE recorded at each tube, without DN
  float truetime[10000000];       // The true time of each hit
  int   vector_index[10000000];   // Index in vector of hits
  int   tubeid[10000000];         // Readout tube ID
  int   mPMTid[10000000];         // Readout tube mPMT ID
  int   mPMT_pmtid[10000000];     // Readout tube mPMT-PMT ID
  int   parentid[10000000];       // Track ID of originating parent, 1 = initial primary particle ( NOT parentID of parent!!)
  int   trackid[10000000];        // Track ID 
  float tube_x[10000000];         // Tube position
  float tube_y[10000000];        
  float tube_z[10000000];        
  float tube_dirx[10000000];      // Hit tube orientation
  float tube_diry[10000000];        
  float tube_dirz[10000000];     
};

struct Cherenkovdigihits_struct {
  int    run;
  int    event;
  int    subevent;
  int    numDigiTubesHit;           // Number of PMTs with digitized hits
  int    totalNumDigiHits;          // Digitized hits per event = hits per Tube x numTubesHit
  float  sumq;                      // Sum of q(readout digitized pe) in event
  float  q[10000000];               // The readout digitized pe
  float  t[10000000];               // The readout digitized time
  int    digivector_index[10000000];// Index in vector of digits
  int    digitubeid[10000000];      // Readout tube ID
  int    digimPMTid[10000000];      // Readout tube mPMT ID
  int    digimPMT_pmtid[10000000];  // Readout tube mPMT-PMT ID
  float  digitube_x[10000000];      // Tube position
  float  digitube_y[10000000];        
  float  digitube_z[10000000];        
  float  digitube_dirx[10000000];   // Hit tube orientation
  float  digitube_diry[10000000];        
  float  digitube_dirz[10000000];        
};

void Setup_tracks_tree( TTree* ttrack, Tracks_struct & tracks ){
  ttrack->SetBranchAddress( "Ntracks", &tracks.nTracks );
  ttrack->SetBranchAddress( "Pid", &tracks.pid );
  ttrack->SetBranchAddress( "Flag", &tracks.flag );
  ttrack->SetBranchAddress( "Mass", &tracks.mass ); 
  ttrack->SetBranchAddress( "P", &tracks.p );
  ttrack->SetBranchAddress( "Energy", &tracks.energy );    
  ttrack->SetBranchAddress( "ParentID", &tracks.parentid );
  ttrack->SetBranchAddress( "TrackID", &tracks.trackid );  
  ttrack->SetBranchAddress( "Time", &tracks.time ); 
  ttrack->SetBranchAddress( "Dirx", &tracks.dir_x );
  ttrack->SetBranchAddress( "Diry", &tracks.dir_y );
  ttrack->SetBranchAddress( "Dirz", &tracks.dir_z );
  ttrack->SetBranchAddress( "Px",   &tracks.pdir_x );
  ttrack->SetBranchAddress( "Py",   &tracks.pdir_y );
  ttrack->SetBranchAddress( "Pz",   &tracks.pdir_z );
  ttrack->SetBranchAddress( "Start_x", &tracks.start_x );
  ttrack->SetBranchAddress( "Start_y", &tracks.start_y );
  ttrack->SetBranchAddress( "Start_z", &tracks.start_z );
  ttrack->SetBranchAddress( "Stop_x", &tracks.stop_x );
  ttrack->SetBranchAddress( "Stop_y" , &tracks.stop_y );
  ttrack->SetBranchAddress( "Stop_z" , &tracks.stop_z );
  ttrack->SetBranchAddress( "Length" , &tracks.length );
}

void Setup_cherenkovhits_tree( TTree * cherenkovHitsTree, Cherenkovhits_struct & hits ){
  cherenkovHitsTree->SetBranchAddress("Run",&hits.run);
  cherenkovHitsTree->SetBranchAddress("Event",&hits.event);
  cherenkovHitsTree->SetBranchAddress("SubEvent",&hits.subevent);
  cherenkovHitsTree->SetBranchAddress("NHits",&hits.totalNumHits);   // #PMTs x #(Ch+DN)hits/PMTs
  cherenkovHitsTree->SetBranchAddress("NHits_noDN",&hits.totalNumHits_noNoise);   // #PMTs x #(Ch+DN)hits/PMTs
  cherenkovHitsTree->SetBranchAddress("NPMTs",&hits.numTubesHit);
  cherenkovHitsTree->SetBranchAddress("NPMTs_noDN",&hits.numTubesHit_noNoise);
  cherenkovHitsTree->SetBranchAddress("Time", &hits.truetime);
  cherenkovHitsTree->SetBranchAddress("PMT_QTot", &hits.totalPe);
  cherenkovHitsTree->SetBranchAddress("PMT_QTot_noDN", &hits.totalPe_noNoise);
  cherenkovHitsTree->SetBranchAddress("ParentID", &hits.parentid);
  cherenkovHitsTree->SetBranchAddress("Vector_index", &hits.vector_index);
  cherenkovHitsTree->SetBranchAddress("Tube", &hits.tubeid);
  cherenkovHitsTree->SetBranchAddress("mPMT", &hits.mPMTid);
  cherenkovHitsTree->SetBranchAddress("mPMT_pmt", &hits.mPMT_pmtid);
  cherenkovHitsTree->SetBranchAddress("TrackID", &hits.trackid);
  cherenkovHitsTree->SetBranchAddress("PMT_x", &hits.tube_x);
  cherenkovHitsTree->SetBranchAddress("PMT_y", &hits.tube_y);
  cherenkovHitsTree->SetBranchAddress("PMT_z", &hits.tube_z);
  cherenkovHitsTree->SetBranchAddress("PMT_dirx", &hits.tube_dirx);
  cherenkovHitsTree->SetBranchAddress("PMT_diry", &hits.tube_diry);
  cherenkovHitsTree->SetBranchAddress("PMT_dirz", &hits.tube_dirz);
}

void Setup_cherenkovdigihits_tree( TTree * cherenkovDigiHitsTree, Cherenkovdigihits_struct & digits ){
  cherenkovDigiHitsTree->SetBranchAddress("Run",&digits.run);
  cherenkovDigiHitsTree->SetBranchAddress("Event",&digits.event);
  cherenkovDigiHitsTree->SetBranchAddress("SubEvent",&digits.subevent);
  cherenkovDigiHitsTree->SetBranchAddress("NDigiHits",&digits.totalNumDigiHits);
  cherenkovDigiHitsTree->SetBranchAddress("NDigiPMTs",&digits.numDigiTubesHit);
  cherenkovDigiHitsTree->SetBranchAddress("QTotDigi",&digits.sumq);
  cherenkovDigiHitsTree->SetBranchAddress("Q", &digits.q);
  cherenkovDigiHitsTree->SetBranchAddress("T", &digits.t);
  cherenkovDigiHitsTree->SetBranchAddress("Vector_index", &digits.digivector_index);
  cherenkovDigiHitsTree->SetBranchAddress("Tube", &digits.digitubeid);
  cherenkovDigiHitsTree->SetBranchAddress("mPMT", &digits.digimPMTid);
  cherenkovDigiHitsTree->SetBranchAddress("mPMT_pmt", &digits.digimPMT_pmtid);
  cherenkovDigiHitsTree->SetBranchAddress("PMT_x", &digits.digitube_x);
  cherenkovDigiHitsTree->SetBranchAddress("PMT_y", &digits.digitube_y);
  cherenkovDigiHitsTree->SetBranchAddress("PMT_z", &digits.digitube_z);
  cherenkovDigiHitsTree->SetBranchAddress("PMT_dirx", &digits.digitube_dirx);
  cherenkovDigiHitsTree->SetBranchAddress("PMT_diry", &digits.digitube_diry);
  cherenkovDigiHitsTree->SetBranchAddress("PMT_dirz", &digits.digitube_dirz);
}

// Function to find index into hits of a particular "tube id", or -1 if not found
int FindHitIndex( const Cherenkovhits_struct & hits, const int tubeid ){
  int index = -1;
   for (int ihit=0; ihit < hits.totalNumHits; ++ihit ){
     if ( hits.tubeid[ihit] == tubeid ) {
       index = ihit;
       break;
     }
   }
   return index;
}

void plot_times_photons(const char* fname="wcsim_isophotons_2p505eV_n5000_flat.root"){
  bool debug = false;


  TFile * fin = new TFile( fname, "read" );

  
  if ( fin==NULL || fin->IsOpen() == false ){
    std::cout<<"Failed to open file "<<fname<<std::endl;
    return;
  }
  
  TTree * ttrack        = (TTree*) fin->Get("Tracks");
  TTree * tcherhits     = (TTree*) fin->Get("CherenkovHits");
  TTree * tcherdigihits = (TTree*) fin->Get("CherenkovDigiHits");

  if ( ttrack == NULL ){
    std::cout<<"Could not get Tracks TTree"<<std::endl;
    retrun;
  }
  if ( tcherhits == NULL ){
    std::cout<<"Could not get CherenkovHits TTree"<<std::endl;
    retrun;
  }
  if ( tcherdigihits == NULL ){
    std::cout<<"Could not get CherenkovDigiHits TTree"<<std::endl;
    retrun;
  }
  
  Tracks_struct tracks;
  Setup_tracks_tree( ttrack, tracks ); 

  Cherenkovhits_struct hits;
  Setup_cherenkovhits_tree( tcherhits, hits ); 

  Cherenkovdigihits_struct ckdigi;
  Setup_cherenkovdigihits_tree( tcherdigihits, ckdigi ); 

  //Timedifference_struct tdiff;
  //Setup_timedifference(ttime_diff, tdiff);


  // Make an output file to make histograms into
  TFile* fout = new TFile("plot_times_photons.root","recreate");

  // times when photons reach PMTs from muon track
  TH1D* htphoton = new TH1D("htphoton","Times Photons reach PMTs from Photons; Time (ns); Counts/bin",
			    140, -20., 150. );
  TH1D* htdigiphoton = new TH1D("htdigiphoton","Digitized Times Photons reach PMTs from Photons; Time (ns); Counts/bin",
			    100, 920., 1120. );
  
  // correlate times when photons reach PMTs with digitized time for muon track.
  TH2D* htvstphoton = new TH2D("htvstphoton","Time from digitizer vs time from hit; Digitizer time (ns); Hit time (ns)",
			       100, 900., 1100., 100, 0., 50. );
  
  //correlate time difference between the true time and hitting time of photons when observed
  //  TH1D* htvstphoton = new TH1D("hcountvstimediff","Time diff vs no. of counts; Time difference (ns); Counts/bin",
  //			       100, 0., 50. );

  // Plots with time corrected by propogation time
  TH1D* htcor = new TH1D("htcor","Times Photons leave initial position; Time (ns); Counts/bin",
			    140, -20., 150. );
  TH1D* htdigicor = new TH1D("htdigcor","Times Photons leave initial point from digitized time; Time (ns); Counts/bin",
			    100, 920., 1120. );

  const TVector3 photonstartpos( 0., -1000.0, 0. ); // cm

  Long64_t nentries = ttrack->GetEntries();

  const float ref_index = 1.33;
  const float oneoverc = 0.033333333; // ns/cm

  for ( Long64_t ientry=0; ientry< nentries; ++ientry){
    if (ientry%50==0) std::cout<<"GetEntry("<<ientry<<") / "<<nentries<<std::endl;
    ttrack->GetEntry( ientry );
    tcherhits->GetEntry( ientry );
    tcherdigihits->GetEntry( ientry );
    //ttime_diff->GetEntry(ientry);
    
    // ignore any "track" or event that is not subevent 1
    if ( hits.subevent != 1 ) continue;
    
    // all "hits" indexed on "totalNumHits"
    if (debug) std::cout<<"  NHits="<<hits.totalNumHits<<std::endl;
    for (int ihit=0; ihit < hits.totalNumHits; ++ihit ){
      if (debug ) {
	if (ihit%500==0){
	  std::cout<<" ihit="<<ihit
		   <<" time="<<hits.truetime[ihit]
		   <<" tubeid="<<hits.tubeid[ihit]
		   <<" trackid="<<hits.trackid[ihit]
		   <<" parentid="<<hits.parentid[ihit]
		   << std::endl;
	}
      }
      htphoton->Fill( hits.truetime[ihit] );
      TVector3 pmtpos(  hits.tube_x[ihit], hits.tube_y[ihit], hits.tube_z[ihit] );
      TVector3 rprop = pmtpos - photonstartpos;
      double dist = rprop.Mag();
      double propog_time;

      propog_time = (dist*ref_index)*oneoverc; // time  = dist * n / velocity
      double time_diff= hits.truetime[ihit] - propog_time;
      htcor->Fill( time_diff );
  
    }    

    // all "ckdigi" indexed on "totalNumDigiHits"
    if (debug) std::cout<<"  NDigits="<<ckdigi.totalNumDigiHits<<std::endl;
    for (int idigit=0; idigit < ckdigi.totalNumDigiHits; ++idigit ){
      if (debug){ 
	if (idigit%50==0){
	  std::cout<<" idigit="<<idigit
		   <<" time="<<ckdigi.t[idigit]
		   <<" tubeid="<<ckdigi.digitubeid[idigit]
		   <<" sumq="<<ckdigi.q[idigit]
		   <<" index into hits="<<ckdigi.digivector_index[idigit]
		 << std::endl;
	}
      }

      TVector3 pmtpos(  ckdigi.digitube_x[idigit], ckdigi.digitube_y[idigit], ckdigi.digitube_z[idigit] );
      TVector3 rprop = pmtpos - photonstartpos;
      double dist = rprop.Mag();
      double propog_time;

      propog_time = (dist*ref_index)*oneoverc; // time  = dist * n / velocity
      double time_diff= ckdigi.t[idigit] - propog_time;
      htdigicor->Fill( time_diff );
      
      htdigiphoton->Fill( ckdigi.t[idigit] );

      int hitindex = FindHitIndex( hits, ckdigi.digitubeid[idigit] );
      if (hitindex != -1 ){
	htvstphoton->Fill( ckdigi.t[idigit] , hits.truetime[ hitindex ] );
	
      }
    }
  }
  
  
  // make an image file of the histograms
  TCanvas * c=new TCanvas("c","c",1000,500);
  c->Divide(2,1);
  c->cd(1);  htphoton->Draw();
  c->cd(2);  htvstphoton->Draw("colz");
  c->Print("plot_times_photons.png");
  
  // make an image file of the histograms
  TCanvas * c2=new TCanvas("c2","c2",1000,500);
  c2->Divide(2,1);
  htcor->SetLineWidth(2);
  htdigicor->SetLineWidth(2);

  htphoton->SetLineWidth(2); htphoton->SetLineColor( kRed );
  htdigiphoton->SetLineWidth(2); htdigiphoton->SetLineColor( kRed );

  c2->cd(1);  
  htcor->Draw();      
  htphoton->Draw("same");
  c2->cd(2);  
  htdigicor->Draw();  
  htdigiphoton->Draw("same");
  c2->Print("plot_cortimes_photons.png");
  


  fout->Write();
  fout->Close();
}

