#ifndef PPsPrimaryGeneratorAction_h
#define PPSPrimaryGeneratorAction_h 1
#include "PPSRunAction.hh"
#include "PPSPrimaryGeneratorMessenger.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

#include "globals.hh"

class G4Event;
class PPSDetectorConstruction;
class PPSRunAction;
class PPSPrimaryGeneratorMessenger;
class G4UIcmdWith3Vector;

class PPSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PPSPrimaryGeneratorAction(PPSDetectorConstruction*);    
  ~PPSPrimaryGeneratorAction();
  
public:
  void GeneratePrimaries(G4Event*);
  
  G4ParticleGun* GetParticleGun() {return particleGun;};
  void SetPrimaryEnergy(G4double E);
  G4double GetPrimaryEnergy();
  void SetOutputFilename(G4String filename);
  G4String GetOutputFilename();
  void SetPrimaryPolarization(G4ThreeVector POL);
  G4ThreeVector GetPrimaryPolarization();
  void SetRndmFlag(G4String val) { rndmFlag = val;}
  void SetZposition(G4double Z)  { itsZ=Z;}	
  G4double GetZposition(){return itsZ;}
  void SetRMS(G4double rms)  {itsRMS=rms;}
  G4double GetRMS() {return itsRMS;}
  void isBatchMode(G4bool flag){itsbatch=flag;}
  G4bool GetBatchMode(){return itsbatch;}
private:
  G4ParticleGun*        				particleGun;
  PPSDetectorConstruction*			detector;
  PPSRunAction*         				pps_runaction;
  PPSPrimaryGeneratorMessenger* 		primaryMessenger;
  G4double itsE;
  G4double itsZ;
  G4double itsRMS;
  G4ThreeVector itsPOL;
  G4bool rndmFlag;
  G4bool itsbatch;
  G4String outputName = "tuple.root";
};

inline void PPSPrimaryGeneratorAction::SetOutputFilename(G4String filename)
{outputName = filename;}

inline G4String PPSPrimaryGeneratorAction::GetOutputFilename()
{return outputName;}

inline void PPSPrimaryGeneratorAction::SetPrimaryEnergy(G4double E)
{itsE = E;}

inline G4double PPSPrimaryGeneratorAction::GetPrimaryEnergy()
{return itsE;}	

inline void PPSPrimaryGeneratorAction::SetPrimaryPolarization(G4ThreeVector POL)
{itsPOL = POL;}

inline G4ThreeVector PPSPrimaryGeneratorAction::GetPrimaryPolarization()
{return itsPOL;}	

#endif


