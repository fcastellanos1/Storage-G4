//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PPSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PPSTargetCaloSD_h
#define PPSTargetCaloSD_h 1

#include "G4VSensitiveDetector.hh"
#include "PPSDetectorConstruction.hh"
#include "PPSTargetCaloHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PPSTargetCaloSD : public G4VSensitiveDetector
{

  public:
		 PPSTargetCaloSD(G4String);//,PPSDetectorConstruction*);
		~PPSTargetCaloSD();

   	     void Initialize(G4HCofThisEvent*HCE);
		 G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
         void EndOfEvent(G4HCofThisEvent*HCE);

  private:
		 PPSTargetCaloHitsCollection* PPSTargetCaloCollection;
		 G4int HCID;
		G4double edep;
};

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
