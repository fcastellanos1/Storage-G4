//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PPSIM code.   
 dadoun@lal.in2p3.fr 
 */
// See Geant4 tips
// http://geant4.slac.stanford.edu/Tips/event/3.html
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PPSTargetCaloHit_h
#define PPSTargetCaloHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

class PPSTargetCaloHit : public G4VHit
{
  public:
  PPSTargetCaloHit();
  PPSTargetCaloHit(G4double x,G4double y,G4double z,G4double depdE=0.,G4double EnWeightZ=0.,G4double EnWeightR=0);
  ~PPSTargetCaloHit();

	PPSTargetCaloHit(const PPSTargetCaloHit&);
   const PPSTargetCaloHit& operator=(const PPSTargetCaloHit&);
   G4int operator==(const PPSTargetCaloHit&) const;

   inline void* operator new(size_t);
   inline void  operator delete(void*);


  private:
  G4double  itsX;
  G4double  itsY;
  G4double 	itsZ;
  G4double 	itsDEPdE;
  G4double 	itsSumDEPdE;
  G4double itsEnergyWeightedPosition;
  G4double itsEnergyWeightedPositionR;

  public:

  inline G4double GetX() const
  {return itsX;}	  
  inline G4double GetY() const
  {return itsY;}	  
  inline G4double GetZ() const
  {return itsZ;}	  
   inline G4double GetDEPdE() const
  {return itsDEPdE;}
  inline G4double GetSumDEPdE() const
  {return itsSumDEPdE;}
  inline G4double GetEnergyWeightedPosition() const
  {return itsEnergyWeightedPosition;}
  inline G4double GetEnergyWeightedPositionR() const
  {return itsEnergyWeightedPositionR;}
 
};
typedef G4THitsCollection<PPSTargetCaloHit> PPSTargetCaloHitsCollection;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
extern G4Allocator<PPSTargetCaloHit> PPSTargetCaloHitAllocator;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline void* PPSTargetCaloHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) PPSTargetCaloHitAllocator.MallocSingle();
  return aHit;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline void PPSTargetCaloHit::operator delete(void *aHit)
{
  PPSTargetCaloHitAllocator.FreeSingle((PPSTargetCaloHit*) aHit);
}
#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
