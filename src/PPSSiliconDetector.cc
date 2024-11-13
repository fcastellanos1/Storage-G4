// local includes 
#include "PPSSiliconDetector.hh"
// geant includes
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4PolarizationHelper.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4StokesVector.hh"

PPSSiliconDetector::PPSSiliconDetector(G4String name):G4VSensitiveDetector(name),StoreHit(true)
{
  G4String HCname = "SiliconDetectorHC";
  collectionName.insert(HCname);
}

PPSSiliconDetector::~PPSSiliconDetector()
{
}

void PPSSiliconDetector::Initialize(G4HCofThisEvent* HCE)
{
	
  PPSSamplerCollection = new PPSSamplerHitsCollection
    (SensitiveDetectorName,collectionName[0]); 
}

G4bool PPSSiliconDetector::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4Track* theTrack		    =	aStep->GetTrack();
  G4StepPoint* preStep      =   aStep->GetPreStepPoint();
  G4StepPoint* postStep     =   aStep->GetPostStepPoint();
  G4double theCharge = theTrack->GetDefinition()->GetPDGCharge();

  if( (StoreHit) )// && (theCharge) ) if you only want e+/e-
    {
      G4ThreeVector pos_i		 =	preStep->GetPosition();
      G4ThreeVector pos_f        =  postStep->GetPosition();
      G4ThreeVector momDir       =	theTrack->GetMomentumDirection();
      G4double energy	         =	aStep->GetTotalEnergyDeposit();

      G4double x			     =	pos_f.x();
      G4double y			     =	pos_f.y();
      G4double z			     =	pos_f.z();
	  
      G4double px			     =	momDir.x();
      G4double py			     =	momDir.y();
      G4double pz			     =	momDir.z();
      G4int    pdg			     =	theTrack->GetDefinition()->GetPDGEncoding();
      G4int    trackID           =   theTrack->GetTrackID();// Primary tracks have a track ID = 1
      G4double time			     =   theTrack->GetGlobalTime();
	  
      G4double x0          = pos_i.x();
      G4double y0          = pos_i.y();
      G4double z0          = pos_i.z();
      G4String sampname    = theTrack->GetVolume()->GetName();
	  
      G4double polX			= 	theTrack->GetPolarization().x();
      G4double polY			= 	theTrack->GetPolarization().y();
      G4double polZ			= 	theTrack->GetPolarization().z();
	  
      PPSSamplerHit* newHit = new PPSSamplerHit(sampname,x0,y0,z0,energy,x,y,z,px,py,pz,pdg,polX,polY,polZ,trackID,time);
      PPSSamplerCollection->insert( newHit );
	  
      if(theTrack->GetVolume()!=theTrack->GetNextVolume()) StoreHit=true;
      else StoreHit=false;
      return true;
    }	
  else
    {
      if(theTrack->GetVolume()!=theTrack->GetNextVolume()) StoreHit=true;
      else StoreHit=false;
      return false;
    }
}

void PPSSiliconDetector::EndOfEvent(G4HCofThisEvent *HCE)
{
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int HCID = SDman->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection( HCID, PPSSamplerCollection );
  
}
