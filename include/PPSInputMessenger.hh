//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/* 
 PPSIM code.   
 dadoun@lal.in2p3.fr 
 */
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifndef PPSInputMessenger_h
#define PPSInputMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PPSInput;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PPSInputMessenger: public G4UImessenger
{
  public:
  
    PPSInputMessenger(PPSInput* );
   ~PPSInputMessenger();
    
   void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    PPSInput*   Input;
    G4UIcmdWithAString*        InputCmd;
    G4bool itsV;
  
  public :  

};


#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

