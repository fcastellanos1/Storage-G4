#ifndef PPSPrimaryGeneratorMessenger_h
#define PPSPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "G4UIcmdWith3Vector.hh"
#include "globals.hh"

class PPSPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

class PPSPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  PPSPrimaryGeneratorMessenger(PPSPrimaryGeneratorAction*);
  virtual ~PPSPrimaryGeneratorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  PPSPrimaryGeneratorAction*   PPSAction;
  G4UIdirectory*               gunDir;
  G4UIcmdWithAString*          outputCmd;
  G4UIcmdWithADoubleAndUnit*   EnergyCmd;
  G4UIcmdWithAString*          RndmCmd;
  G4UIcmdWith3Vector*          PolarizationCmd;
  G4UIcmdWithADoubleAndUnit*   ZpositionCmd;
  G4UIcmdWithADouble*          GaussianRMSCmd;
};


#endif

