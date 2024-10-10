#ifndef PPSPhysicsListMessenger_h
#define PPSPhysicsListMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PPSPhysicsList;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithoutParameter;

class PPSPhysicsListMessenger: public G4UImessenger
{
  public:
  
    PPSPhysicsListMessenger(PPSPhysicsList* );
   ~PPSPhysicsListMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    PPSPhysicsList*               pPhysicsList;
    
    G4UIdirectory*             physDir;
    G4UIcmdWithADoubleAndUnit* gammaCutCmd;
    G4UIcmdWithADoubleAndUnit* electCutCmd;
    G4UIcmdWithADoubleAndUnit* protoCutCmd;    
    G4UIcmdWithADoubleAndUnit* allCutCmd;
    G4UIcmdWithAString*        pListCmd;
    G4UIcmdWithoutParameter* PhotoElectroNuclearCmd;
	
};
#endif
