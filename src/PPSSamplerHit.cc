//local
#include "PPSSamplerHit.hh"

// geant4
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<PPSSamplerHit> SamplerHitAllocator;

PPSSamplerHit::PPSSamplerHit() {
}

PPSSamplerHit::PPSSamplerHit(G4String aName,G4double x0,G4double y0,G4double z0,
			     G4double energy,G4double x,G4double y, G4double z,
			     G4double px, G4double py, G4double pz, G4int pdg,
			     G4double Sx,G4double Sy,G4double Sz,G4int trackID,G4double time)
  :itsName(aName),itsZ0(z0),itsY0(y0),itsX0(x0),
   itsEnergy(energy),itsX(x),itsY(y),itsZ(z),
   itsPX(px),itsPY(py),itsPZ(pz),itsPDG(pdg),
   itsSx(Sx),itsSy(Sy),itsSz(Sz),itsTrackID(trackID),itsTime(time) {
}

PPSSamplerHit::~PPSSamplerHit() {
}

PPSSamplerHit::PPSSamplerHit(const PPSSamplerHit& right)
{
  itsName = right.itsName;
  itsZ0 = right.itsZ0;
  itsY0 = right.itsY0;
  itsX0 = right.itsX0;
  itsTime = right.itsTime; 
  
  itsEnergy = right.itsEnergy;
  itsX = right.itsX;
  itsY = right.itsY;
  itsZ = right.itsZ;
  itsPX = right.itsPX;
  itsPY = right.itsPY;
  itsPZ = right.itsPZ;
  itsPDG = right.itsPDG;
  itsSx = right.itsSx;	
  itsSy = right.itsSy;	
  itsSz = right.itsSz;	
  itsTrackID=right.itsTrackID;
}

const PPSSamplerHit& PPSSamplerHit::operator=(const PPSSamplerHit& right)
{
  itsZ0 = right.itsZ0;
  itsY0 = right.itsY0;
  itsX0 = right.itsX0;
  itsTime = right.itsTime;
  itsName= right.itsName;
  
  itsEnergy = right.itsEnergy;
  itsX = right.itsX;
  itsY = right.itsY;
  itsZ = right.itsZ;
  itsPX = right.itsPX;
  itsPY = right.itsPY;
  itsPZ = right.itsPZ;
  itsPDG = right.itsPDG;
  itsSx = right.itsSx;	
  itsSy = right.itsSy;	
  itsSz = right.itsSz;	
  itsTrackID = right.itsTrackID;
  
  return *this;
}
