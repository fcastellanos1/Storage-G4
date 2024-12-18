//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PPSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//#include "PPSOutput.hh"
#include "PPSTargetCaloHit.hh"
#include "PPSDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4RotationMatrix.hh"
#include "G4Box.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4ios.hh"

//extern PPSOutput rootOutput;
G4Allocator<PPSTargetCaloHit> PPSTargetCaloHitAllocator;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PPSTargetCaloHit::PPSTargetCaloHit()
{;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PPSTargetCaloHit::PPSTargetCaloHit(G4double x,G4double y,G4double z,G4double depdE,G4double EnWeightZ,G4double EnWeightR)
:itsX(x),itsY(y),itsZ(z),itsDEPdE(depdE),itsEnergyWeightedPosition(EnWeightZ),itsEnergyWeightedPositionR(EnWeightR)
{;}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PPSTargetCaloHit::~PPSTargetCaloHit()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PPSTargetCaloHit::PPSTargetCaloHit(const PPSTargetCaloHit& right)
{
  
  itsX     = right.itsX;
  itsY     = right.itsY;
  itsZ     = right.itsZ;

  itsDEPdE = right.itsDEPdE;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const PPSTargetCaloHit& PPSTargetCaloHit::operator=(const PPSTargetCaloHit& right)
{
  itsX     = right.itsX;
  itsY     = right.itsY;
  itsZ     = right.itsZ;

  itsDEPdE = right.itsDEPdE;
		  
  return *this;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
