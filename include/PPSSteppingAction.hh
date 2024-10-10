//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PPSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PPSSteppingAction_h
#define PPSSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class PPSDetectorConstruction;
class PPSPrimaryGeneratorAction;
class PPSRunAction;
class PPSEventAction;


class PPSSteppingAction : public G4UserSteppingAction
{
  public:
    PPSSteppingAction(PPSDetectorConstruction*, PPSPrimaryGeneratorAction*, PPSRunAction*, PPSEventAction*);
   ~PPSSteppingAction();
    
     G4double total_dep;
    void UserSteppingAction(const G4Step*);
  
  private:
    PPSDetectorConstruction*   detector;
    PPSPrimaryGeneratorAction* primary;
    PPSRunAction*              runAction;
    PPSEventAction*            eventAction;
	G4double tot_target;
	G4double tot_sampler;
	G4int MaxStepNumber;
};

#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......