#ifndef PPSSamplerHit_h
#define PPSSamplerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class PPSSamplerHit : public G4VHit
{
public:
    
    PPSSamplerHit();
    PPSSamplerHit(G4String aName,G4double z0,G4double y0,G4double x0,
                  G4double energy,G4double x, G4double y, G4double z,
                  G4double px, G4double py, G4double pz,G4int pdg,G4double Sx,
                  G4double Sy,G4double Sz,G4int trackID,G4double time, G4double Edep=0.);
    
    ~PPSSamplerHit();
    PPSSamplerHit(const PPSSamplerHit&);
    const PPSSamplerHit& operator=(const PPSSamplerHit&);
    G4int operator==(const PPSSamplerHit&) const;
    
    inline void* operator new(size_t);
    inline void  operator delete(void*);
    
private:
    G4String itsName;
    G4double itsZ0;
    G4double itsY0;
    G4double itsX0;
    G4double itsEnergy;
    G4double itsX;
    G4double itsY;
    G4double itsZ;
    G4double itsPX;
    G4double itsPY;
    G4double itsPZ;
    G4int    itsPDG;
    G4double itsSx;
    G4double itsSy;
    G4double itsSz;
    G4int    itsTrackID;
    G4double itsTime;
    G4double itsEdep;
    
public:
    inline void SetName(G4String aName)
    {itsName=aName;}
    
    inline G4String GetName() const
    {return itsName;}
    inline G4double GetZ0() const
    {return itsZ0;}
    inline G4double GetY0() const
    {return itsY0;}
    inline G4double GetX0() const
    {return itsX0;}
    inline G4double GetEnergy() const
    {return itsEnergy;}
    inline G4double GetX() const
    {return itsX;}  
    inline G4double GetY() const
    {return itsY;}  
    inline G4double GetZ() const
    {return itsZ;}  
    inline G4double GetPX() const
    {return itsPX;}  
    inline G4double GetPY() const
    {return itsPY;}
    inline G4double GetPZ() const
    {return itsPZ;}  
    inline G4int GetPDG() const
    {return itsPDG;}
    inline G4double GetSx() const
    {return itsSx;}
    inline G4double GetSy() const
    {return itsSy;}
    inline G4double GetSz() const
    {return itsSz;}
    inline G4int GetTrackID() const
    {return itsTrackID;}
    inline G4double GetTime() const
    {return itsTime;}
    inline G4double GetEdep() const
    {return itsEdep;}
};

typedef G4THitsCollection<PPSSamplerHit> PPSSamplerHitsCollection;

extern G4Allocator<PPSSamplerHit> SamplerHitAllocator;

inline void* PPSSamplerHit::operator new(size_t)
{
    void *aHit;
    aHit = (void *) SamplerHitAllocator.MallocSingle();
    return aHit;
}

inline void PPSSamplerHit::operator delete(void *aHit)
{
    SamplerHitAllocator.FreeSingle((PPSSamplerHit*) aHit);
}

#endif
