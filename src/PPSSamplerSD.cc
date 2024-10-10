// local includes 
#include "PPSSamplerSD.hh"
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

PPSSamplerSD::PPSSamplerSD(G4String name):G4VSensitiveDetector(name),StoreHit(true)
{
  G4String HCname;
  collectionName.insert(HCname="SamplerCollection");
}

PPSSamplerSD::~PPSSamplerSD()
{
}

void PPSSamplerSD::Initialize(G4HCofThisEvent* HCE)
{
	
  PPSSamplerCollection = new PPSSamplerHitsCollection
    (SensitiveDetectorName,collectionName[0]); 
}

G4bool PPSSamplerSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4Track* theTrack		    =	aStep->GetTrack();
  //G4cout << "found track" << theTrack << " store ? " << StoreHit << G4endl;
  if(StoreHit)
    {
      G4ThreeVector pos		 =	theTrack->GetPosition();
      G4ThreeVector momDir =	theTrack->GetMomentumDirection();
      G4ThreeVector vtx		 =	theTrack->GetVertexPosition();
      G4double energy	     =	theTrack->GetTotalEnergy();

      G4double x			     =	pos.x();
      G4double y			     =	pos.y();
      G4double z			     =	pos.z();
	  
      G4double px			     =	momDir.x();
      G4double py			     =	momDir.y();
      G4double pz			     =	momDir.z();
      G4int    pdg			   =	theTrack->GetDefinition()->GetPDGEncoding();
      G4int    trackID     =   theTrack->GetTrackID();// Primary tracks have a track ID = 1
      G4double time			   =   theTrack->GetGlobalTime();
	  
      G4double x0          =  vtx.x();
      G4double y0          =  vtx.y();
      G4double z0          =  vtx.z();
      G4String sampname    =  theTrack->GetVolume()->GetName();
	  
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

void PPSSamplerSD::EndOfEvent(G4HCofThisEvent *HCE)
{
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int HCID = SDman->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection( HCID, PPSSamplerCollection );
  
}
