#include "WCSimPrimaryGeneratorAction.hh"
#include "WCSimDetectorConstruction.hh"
#include "WCSimPrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "Randomize.hh"
#include <fstream>
#include <vector>
#include <string>

#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

using std::vector;
using std::string;
using std::fstream;

vector<string> tokenize( string separators, string input );

inline vector<string> readInLine(fstream& inFile, int lineSize, char* inBuf)
{
    // Read in line break it up into tokens
    inFile.getline(inBuf,lineSize);
    return tokenize(" $", inBuf);
}

inline float atof( const string& s ) {return std::atof( s.c_str() );}
inline int   atoi( const string& s ) {return std::atoi( s.c_str() );}

WCSimPrimaryGeneratorAction::WCSimPrimaryGeneratorAction(
        WCSimDetectorConstruction* myDC)
:myDetector(myDC)
{
    //T. Akiri: Initialize GPS to allow for the laser use 
    MyGPS = new G4GeneralParticleSource();

    // Initialize to zero
    mode = 0;
    vtxvol = 0;
    vtx = G4ThreeVector(0.,0.,0.);
    nuEnergy = 0.;
    _counterRock=0; // counter for generated in Rock
    _counterCublic=0; // counter generated

    //---Set defaults. Do once at beginning of session.

    G4int n_particle = 1;
    particleGun = new G4ParticleGun(n_particle);
    particleGun->SetParticleEnergy(1.0*GeV);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.0));

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    particleGun->
        SetParticleDefinition(particleTable->FindParticle(particleName="mu+"));

    particleGun->
        SetParticlePosition(G4ThreeVector(0.*m,0.*m,0.*m));

    messenger = new WCSimPrimaryGeneratorMessenger(this);

    useMulineEvt = true;
    useNeutEvt = false;
    useNormalEvt = false;
    useLaserEvt = false;

    fEvNum = 0;
    fRooTrackerTree = NULL;
    fInputNeutFile = NULL;
    fNEntries = 0;
}

WCSimPrimaryGeneratorAction::~WCSimPrimaryGeneratorAction()
{
    if (IsGeneratingVertexInRock()){
        G4cout << "Fraction of Rock volume is : " << G4endl;
        G4cout << " Random number generated in Rock / in Cublic = " 
            << _counterRock << "/" << _counterCublic 
            << " = " << _counterRock/(G4double)_counterCublic << G4endl;
    }
    inputFile.close();

    if(useNeutEvt) delete fRooTrackerTree;

    delete particleGun;
    delete MyGPS;   //T. Akiri: Delete the GPS variable
    delete messenger;
}

void WCSimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

    // We will need a particle table
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

    // Temporary kludge to turn on/off vector text format 
    G4bool useNuanceTextFormat = true;

    // Do for every event
    if (useMulineEvt)
    { 

        if ( !inputFile.is_open() )
        {
            G4cout << "Set a vector file using the command /mygen/vecfile name"
                << G4endl;
            return;
        }

        if (useNuanceTextFormat)
        {
            const int lineSize=100;
            char      inBuf[lineSize];
            vector<string> token(1);

            token = readInLine(inputFile, lineSize, inBuf);

            if (token.size() == 0) 
            {
                G4cout << "end of nuance vector file!" << G4endl;
            }
            else if (token[0] != "begin")
            {
                G4cout << "unexpected line begins with " << token[0] << G4endl;
            }
            else   // normal parsing begins here
            {
                // Read the nuance line (ignore value now)

                token = readInLine(inputFile, lineSize, inBuf);
                mode = atoi(token[1]);

                // Read the Vertex line
                token = readInLine(inputFile, lineSize, inBuf);
                vtx = G4ThreeVector(atof(token[1])*cm,
                        atof(token[2])*cm,
                        atof(token[3])*cm);

                // true : Generate vertex in Rock , false : Generate vertex in WC tank
                SetGenerateVertexInRock(false);

                // Next we read the incoming neutrino and target

                // First, the neutrino line

                token=readInLine(inputFile, lineSize, inBuf);
                beampdg = atoi(token[1]);
                beamenergy = atof(token[2])*MeV;
                beamdir = G4ThreeVector(atof(token[3]),
                        atof(token[4]),
                        atof(token[5]));

                // Now read the target line

                token=readInLine(inputFile, lineSize, inBuf);
                targetpdg = atoi(token[1]);
                targetenergy = atof(token[2])*MeV;
                targetdir = G4ThreeVector(atof(token[3]),
                        atof(token[4]),
                        atof(token[5]));

                // Read the info line, basically a dummy
                token=readInLine(inputFile, lineSize, inBuf);
                G4cout << "Vector File Record Number " << token[2] << G4endl;
                vecRecNumber = atoi(token[2]);

                // Now read the outgoing particles
                // These we will simulate.


                while ( token=readInLine(inputFile, lineSize, inBuf),
                        token[0] == "track" )
                {
                    // We are only interested in the particles
                    // that leave the nucleus, tagged by "0"


                    if ( token[6] == "0")
                    {
                        G4int pdgid = atoi(token[1]);
                        G4double energy = atof(token[2])*MeV;
                        G4ThreeVector dir = G4ThreeVector(atof(token[3]),
                                atof(token[4]),
                                atof(token[5]));
                        particleGun->
                            SetParticleDefinition(particleTable->
                                    FindParticle(pdgid));
                        G4double mass = 
                            particleGun->GetParticleDefinition()->GetPDGMass();

                        G4double ekin = energy - mass;

                        particleGun->SetParticleEnergy(ekin);
                        //G4cout << "Particle: " << pdgid << " KE: " << ekin << G4endl;
                        particleGun->SetParticlePosition(vtx);
                        particleGun->SetParticleMomentumDirection(dir);
                        particleGun->GeneratePrimaryVertex(anEvent);
                    }
                }
            }
        }
        else 
        {    // old muline format  
            inputFile >> nuEnergy >> energy >> xPos >> yPos >> zPos 
                >> xDir >> yDir >> zDir;

            G4double random_z = ((myDetector->GetWaterTubePosition())
                    - .5*(myDetector->GetWaterTubeLength()) 
                    + 1.*m + 15.0*m*G4UniformRand())/m;
            zPos = random_z;
            G4ThreeVector vtx = G4ThreeVector(xPos, yPos, random_z);
            G4ThreeVector dir = G4ThreeVector(xDir,yDir,zDir);

            particleGun->SetParticleEnergy(energy*MeV);
            particleGun->SetParticlePosition(vtx);
            particleGun->SetParticleMomentumDirection(dir);
            particleGun->GeneratePrimaryVertex(anEvent);
        }
    }

    else if (useNeutEvt)
    { 
        if ( !inputFile.is_open() )
        {
            G4cout << "Set a NEUT vector file using the command /mygen/vecfile name"
                << G4endl;
            return;
        }

        fNVtx = 0;

        //Generate 1 event
        SetupBranchAddresses(tempVtx); //link tempVtx and current input file

        //Load event from file
        if (fEvNum<fNEntries){
            fRooTrackerTree->GetEntry(fEvNum);
            fEvNum++;
        }
        else{
            G4cout << "End of File" << G4endl; 
            EndOfFile=true; 
            G4RunManager::GetRunManager()->AbortRun(); 
            return; 
        }

        //Position in WCSim coordinates
        xPos = tempVtx->fEvNumtVtx[0]*m;
        yPos = tempVtx->fEvNumtVtx[1]*m;
        zPos = tempVtx->fEvNumtVtx[2]*m;

        //Check if event is outside detector; skip to next event if so; keep
        //loading events until one is found within the detector or there are
        //no more interaction to simulate for this event.
        //The current neut vector files do not correspond directly to the detector dimensions, so only keep those events within the detector
        while (sqrt(pow(xPos,2)+pow(yPos,2))>myDetector->GetWCIDDiameter()/2. || (abs(zPos)>myDetector->GetWCIDHeight()/2)){
            //Load another event
            if (fEvNum<fNEntries){
                fRooTrackerTree->GetEntry(fEvNum);
                G4cout << "Skipped event# " << fEvNum-1 << " in file " << file << " (event vertex outside detector)" << G4endl;
                fEvNum++;
            }
            else{
                G4cout << "End of File" << G4endl; 
                EndOfFile=true; 
                G4RunManager::GetRunManager()->AbortRun(); 
                return; 
            }
            //Convert coordinates
            xPos = tempVtx->fEvNumtVtx[0]*m; 
            yPos = tempVtx->fEvNumtVtx[1]*m; 
            zPos = tempVtx->fEvNumtVtx[2]*m;
        }

        //Generate particles
        //i = 0 is the neutrino
        //i = 1 is the target nucleus
        //i = 2 is the target nucleon
        //i > 2 are the outgoing particles
        for (int i = 3; i < tempVtx->StdHepN; i++){

            xDir=tempVtx->StdHepP4[i][0];
            yDir=tempVtx->StdHepP4[i][1];
            zDir=tempVtx->StdHepP4[i][2];

            momentum=sqrt(pow(xDir,2)+pow(yDir,2)+pow(zDir,2))*GeV;

            Time=GetTime(nBunches,sigma,bunchSep);

            G4ThreeVector vtx = G4ThreeVector(xPos, yPos, zPos);
            G4ThreeVector dir = G4ThreeVector(xDir, yDir, zDir);

            particleGun->SetParticleDefinition(particleTable->FindParticle(tempVtx->StdHepPdg[i]));
            particleGun->SetParticleMomentum(momentum);
            particleGun->SetParticlePosition(vtx);
            particleGun->SetParticleMomentumDirection(dir);
            particleGun->SetParticleTime(Time);
            particleGun->GeneratePrimaryVertex(anfEvNument);  //Place vertex in stack
        }

        //Save NEUT vertex truth info in TClonesArray entry
        //Currently only generating one interaction per event, but
        //will want multiple interactions when simulating near detectors, therefore use TClonesArray to store NRooTracker objects
        fCurrNeutVtx = new ((*fVertices)[fNVtx]) NRooTrackerVtx();
        fNVtx += 1;
        fCurrNeutVtx->Copy(tempVtx);
        fCurrNeutVtx->TruthVertexID = -999;

        //Add event to truth info tree
        fRooTrackerOutputTree->Fill();
        fVertices->Clear();
    }

    else if (useNormalEvt)
    {      // manual gun operation
        particleGun->GeneratePrimaryVertex(anEvent);

        G4ThreeVector P  =anEvent->GetPrimaryVertex()->GetPrimary()->GetMomentum();
        G4ThreeVector vtx=anEvent->GetPrimaryVertex()->GetPosition();
        G4double m       =anEvent->GetPrimaryVertex()->GetPrimary()->GetMass();
        G4int pdg        =anEvent->GetPrimaryVertex()->GetPrimary()->GetPDGcode();

        G4ThreeVector dir  = P.unit();
        G4double E         = std::sqrt((P.dot(P))+(m*m));

        //     particleGun->SetParticleEnergy(E);
        //     particleGun->SetParticlePosition(vtx);
        //     particleGun->SetParticleMomentumDirection(dir);

        SetVtx(vtx);
        SetBeamEnergy(E);
        SetBeamDir(dir);
        SetBeamPDG(pdg);
    }
    else if (useLaserEvt)
    {
        //T. Akiri: Create the GPS LASER event
        MyGPS->GeneratePrimaryVertex(anEvent);

        G4ThreeVector P   =anEvent->GetPrimaryVertex()->GetPrimary()->GetMomentum();
        G4ThreeVector vtx =anEvent->GetPrimaryVertex()->GetPosition();
        G4int pdg         =anEvent->GetPrimaryVertex()->GetPrimary()->GetPDGcode();

        G4ThreeVector dir  = P.unit();
        G4double E         = std::sqrt((P.dot(P)));

        SetVtx(vtx);
        SetBeamEnergy(E);
        SetBeamDir(dir);
        SetBeamPDG(pdg);
    }
}

// Returns a vector with the tokens
vector<string> tokenize( string separators, string input ) 
{
    unsigned int startToken = 0, endToken; // Pointers to the token pos
    vector<string> tokens;  // Vector to keep the tokens

    if( separators.size() > 0 && input.size() > 0 ) 
    {

        while( startToken < input.size() )
        {
            // Find the start of token
            startToken = input.find_first_not_of( separators, startToken );

            // If found...
            if( startToken != input.npos ) 
            {
                // Find end of token
                endToken = input.find_first_of( separators, startToken );
                if( endToken == input.npos )
                    // If there was no end of token, assign it to the end of string
                    endToken = input.size();

                // Extract token
                tokens.push_back( input.substr( startToken, endToken - startToken ) );

                // Update startToken
                startToken = endToken;
            }
        }
    }

    return tokens;
}

void WCSimPrimaryGeneratorAction::InitialiseNeutObjects()
{
    //Setup TClonesArray to store NEUT truth info
    fVertices = new TClonesArray("NRooTrackerVtx", 1);
    fVertices->BypassStreamer();
    fVertices->Clear();

    fRooTrackerOutputTree = new TTree("fRooTrackerOutputTree","Event Vertex Truth Array");
    fRooTrackerOutputTree->Branch("NVtx",&fNVtx,"NVtx/I");
    fRooTrackerOutputTree->Branch("NRooTrackerVtx","TClonesArray", &fVertices);
    fNVtx = 0;

    tempVtx = new NRooTrackerVtx();
}

void WCSimPrimaryGeneratorAction::OpenNeutFile(G4String fileName)
{
    if (fInputNeutFile) fInputNeutFile->Delete();

    fInputNeutFile = TFile::Open(fileName.data());
    if (!fInputNeutFile){ 
        G4cout << "Cannot open: " << fileName << G4endl; 
        exit(1);
    }

    fRooTrackerTree = (TTree*) fInputNeutFile->Get("nRooTracker");
    if (!fRooTrackerTree){
        G4cout << "File: " << fileName << " does not contain a NEUT nRooTracker tree - please check you intend to process NEUT events" << G4endl;
        exit(1);
    }
    fNEntries=fRooTrackerTree->GetEntries();
}

void WCSimPrimaryGeneratorAction::SetupBranchAddresses(NRooTrackerVtx* nrootrackervtx){

    // Set up branch address for rooTrackerVertex tree in nuPRISM files
    fRooTrackerTree->SetBranchAddress("EvtCode",        &(nrootrackervtx->EvtCode) );
    fRooTrackerTree->SetBranchAddress("EvtNum",         &(nrootrackervtx->EvtNum) );
    fRooTrackerTree->SetBranchAddress("EvtXSec",        &(nrootrackervtx->EvtXSec) );
    fRooTrackerTree->SetBranchAddress("EvtDXSec",       &(nrootrackervtx->EvtDXSec) );
    fRooTrackerTree->SetBranchAddress("EvtWght",        &(nrootrackervtx->EvtWght) );
    fRooTrackerTree->SetBranchAddress("EvtProb",        &(nrootrackervtx->EvtProb) );
    fRooTrackerTree->SetBranchAddress("EvtVtx",          (nrootrackervtx->EvtVtx) );
    fRooTrackerTree->SetBranchAddress("StdHepN",        &(nrootrackervtx->StdHepN) );
    fRooTrackerTree->SetBranchAddress("StdHepPdg",       (nrootrackervtx->StdHepPdg) );
    fRooTrackerTree->SetBranchAddress("StdHepStatus",   &(nrootrackervtx->StdHepStatus) );
    fRooTrackerTree->SetBranchAddress("StdHepX4",        (nrootrackervtx->StdHepX4) );
    fRooTrackerTree->SetBranchAddress("StdHepP4",        (nrootrackervtx->StdHepP4) );
    fRooTrackerTree->SetBranchAddress("StdHepPolz",      (nrootrackervtx->StdHepPolz) );
    fRooTrackerTree->SetBranchAddress("StdHepFd",        (nrootrackervtx->StdHepFd) );
    fRooTrackerTree->SetBranchAddress("StdHepLd",        (nrootrackervtx->StdHepLd) );
    fRooTrackerTree->SetBranchAddress("StdHepFm",        (nrootrackervtx->StdHepFm) );
    fRooTrackerTree->SetBranchAddress("StdHepLm",        (nrootrackervtx->StdHepLm) );

    // NEUT > v5.0.7 && MCP > 1 (>10a)
    fRooTrackerTree->SetBranchAddress("NEnvc",          &(nrootrackervtx->NEnvc)    );
    fRooTrackerTree->SetBranchAddress("NEipvc",          (nrootrackervtx->NEipvc)   );
    fRooTrackerTree->SetBranchAddress("NEpvc",           (nrootrackervtx->NEpvc)    );
    fRooTrackerTree->SetBranchAddress("NEiorgvc",        (nrootrackervtx->NEiorgvc) );
    fRooTrackerTree->SetBranchAddress("NEiflgvc",        (nrootrackervtx->NEiflgvc) );
    fRooTrackerTree->SetBranchAddress("NEicrnvc",        (nrootrackervtx->NEicrnvc) );

    fRooTrackerTree->SetBranchAddress("NEnvert",        &(nrootrackervtx->NEnvert)  );
    fRooTrackerTree->SetBranchAddress("NEposvert",       (nrootrackervtx->NEposvert) );
    fRooTrackerTree->SetBranchAddress("NEiflgvert",      (nrootrackervtx->NEiflgvert) );
    fRooTrackerTree->SetBranchAddress("NEnvcvert",      &(nrootrackervtx->NEnvcvert) );
    fRooTrackerTree->SetBranchAddress("NEdirvert",       (nrootrackervtx->NEdirvert) );
    fRooTrackerTree->SetBranchAddress("NEabspvert",      (nrootrackervtx->NEabspvert) );
    fRooTrackerTree->SetBranchAddress("NEabstpvert",     (nrootrackervtx->NEabstpvert) );
    fRooTrackerTree->SetBranchAddress("NEipvert",        (nrootrackervtx->NEipvert)  );
    fRooTrackerTree->SetBranchAddress("NEiverti",        (nrootrackervtx->NEiverti)  );
    fRooTrackerTree->SetBranchAddress("NEivertf",        (nrootrackervtx->NEivertf)  );
    // end NEUT > v5.0.7 && MCP > 1 (>10a)

    // NEUT > v5.1.2 && MCP >= 5
    fRooTrackerTree->SetBranchAddress("NEcrsx",          &(nrootrackervtx->NEcrsx)    );
    fRooTrackerTree->SetBranchAddress("NEcrsy",          &(nrootrackervtx->NEcrsy)    );
    fRooTrackerTree->SetBranchAddress("NEcrsz",          &(nrootrackervtx->NEcrsz)    );
    fRooTrackerTree->SetBranchAddress("NEcrsphi",        &(nrootrackervtx->NEcrsphi)    );

}

