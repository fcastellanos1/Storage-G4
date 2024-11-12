#ifndef PPSSiliconDetector_h
#define PPSSiliconDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "PPSSamplerHit.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PPSSiliconDetector : public G4VSensitiveDetector
{
  public:
     PPSSiliconDetector(G4String);
    ~PPSSiliconDetector();

    void Initialize(G4HCofThisEvent*HCE);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*HCE);

  private:
    PPSSamplerHitsCollection* PPSSamplerCollection;
    G4Navigator* StepperNavigator;
    G4bool StoreHit;
};

#endif
