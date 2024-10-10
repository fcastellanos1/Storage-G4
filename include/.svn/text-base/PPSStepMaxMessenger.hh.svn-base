//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PPSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PPSStepMaxMessenger_h
#define PPSStepMaxMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PPSStepMax;
class G4UIcmdWithADoubleAndUnit;

class PPSStepMaxMessenger: public G4UImessenger
{
  public:
    PPSStepMaxMessenger(PPSStepMax*);
   ~PPSStepMaxMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    PPSStepMax* pStepMax;
    G4UIcmdWithADoubleAndUnit* StepMaxCmd;
};



#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......